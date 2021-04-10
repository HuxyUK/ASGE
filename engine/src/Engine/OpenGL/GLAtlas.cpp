//  Copyright (c) 2021 James Huxtable. All rights reserved.
//
//  This work is licensed under the terms of the MIT license.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#include "GLAtlas.hpp"

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <iostream>

ASGE::FontTextureAtlas::FontTextureAtlas()
  : texture(0), width(0), height(0) {}

ASGE::FontTextureAtlas::~FontTextureAtlas()
{
  if (glfwGetCurrentContext() != nullptr)
  {
    glDeleteTextures(1, &texture);
  }
}

bool ASGE::FontTextureAtlas::init(const FT_Face& face, int h)
{
  FT_Set_Pixel_Sizes(face, 0, h);
  memset(characters, 0, sizeof(Character));
  calculateTextureSize(face);

  if (width == 0 && height == 0)
  {
    return false;
  }

  if (!generateTexture() || !calculateFontFace(face))
  {
    return false;
  }

  Logging::DEBUG(std::string("Generated Font Atlas: ").append(face->family_name));
  std::stringstream ss;
  ss << "Generated a " << width << "x " << height << " (" << width * height / 1024
     << " kb) texture atlas";
  Logging::DEBUG(ss.str());
  return true;
}

GLuint ASGE::FontTextureAtlas::getTextureID() const noexcept
{
  return texture;
}

const ASGE::Character& ASGE::FontTextureAtlas::getCharacter(int idx) const
{
  return characters[idx];
}

void ASGE::FontTextureAtlas::calculateTextureSize(const FT_Face& face)
{
  FT_GlyphSlot glyph_slot = face->glyph;

  unsigned int roww = 0;
  unsigned int rowh = 0;

  width  = 0;
  height = 0;

  // run through ASCII and calculate texture size
  for (int i = 32; i < 128; i++)
  {
    if (FT_Load_Char(face, i, FT_LOAD_RENDER) != 0)
    {
      Logging::ERRORS("FT: Loading char " + std::to_string(i) + "failed");
      continue;
    }

    // ensure the next glpyh would fit in 1024 width
    if (roww + glyph_slot->bitmap.width + 1 >= 1024)
    {
      width = std::max(width, roww);
      height += rowh;
      roww = 0;
      rowh = 0;
    }

    roww += glyph_slot->bitmap.width + 1;
    rowh = std::max(rowh, glyph_slot->bitmap.rows);
  }

  // set the texture's width and height
  width = std::max(width, roww);
  height += rowh;
}

bool ASGE::FontTextureAtlas::generateTexture()
{
  glActiveTexture(GL_TEXTURE0);
  GLVCMD(glGenTextures, 1, &texture);
  GLVCMD(glActiveTexture, GL_TEXTURE0 + texture);

  GLVMSG(__PRETTY_FUNCTION__, glBindTexture, GL_TEXTURE_2D, texture);
  // GLVCMD(glBindTexture, GL_TEXTURE_2D, texture);

  GLVCMD(
    glTexImage2D, GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

  return true;
}

void ASGE::FontTextureAtlas::setSampleParams()
{
  GLVMSG(__PRETTY_FUNCTION__, glPixelStorei, GL_UNPACK_ALIGNMENT, 1);
  GLVMSG(__PRETTY_FUNCTION__, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GLVMSG(__PRETTY_FUNCTION__, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GLVMSG(__PRETTY_FUNCTION__, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  GLVMSG(__PRETTY_FUNCTION__, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  float ansio_levels;
  GLVMSG(__PRETTY_FUNCTION__, glGetFloatv, GL_MAX_TEXTURE_MAX_ANISOTROPY, &ansio_levels);
  GLVMSG(__PRETTY_FUNCTION__, glTexParameterf, GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, ansio_levels);
}

bool ASGE::FontTextureAtlas::calculateFontFace(const FT_Face& face)
{
  setSampleParams();

  auto glyph_slot         = face->glyph;
  unsigned int x          = 0;
  unsigned int y          = 0;
  unsigned int row_height = 0;

  for (int i = 32; i < 128; i++)
  {
    if (FT_Load_Char(face, i, FT_LOAD_RENDER))
    {
      std::cout << "Loading character " << char(i) << " failed\n";
      continue;
    }

    if (x + glyph_slot->bitmap.width + 1 >= 1024)
    {
      y += row_height;
      row_height = 0;
      x          = 0;
    }

    glTexSubImage2D(
      GL_TEXTURE_2D,
      0,
      x,
      y,
      glyph_slot->bitmap.width,
      glyph_slot->bitmap.rows,
      GL_RED,
      GL_UNSIGNED_BYTE,
      glyph_slot->bitmap.buffer);

    auto& c = characters[i];

    // Bitshift by 6 to get value in pixels (2^6 = 64)
    c.Advance.x = glyph_slot->advance.x >> 6;
    c.Advance.y = glyph_slot->advance.y >> 6;

    c.Size.x = glyph_slot->bitmap.width;
    c.Size.y = glyph_slot->bitmap.rows;

    c.Bearing.x = glyph_slot->bitmap_left;
    c.Bearing.y = glyph_slot->bitmap_top;

    float uv_offset_x = (float)x / (float)width;
    float uv_offset_y = (float)y / (float)height;
    float uv_width    = c.Size.x / (float)width;
    float uv_height   = c.Size.y / (float)height;

    c.UV.x = uv_offset_x;
    c.UV.y = uv_offset_y;
    c.UV.z = uv_offset_x + uv_width;
    c.UV.w = uv_offset_y + uv_height;

    row_height = std::max(row_height, glyph_slot->bitmap.rows);
    x += glyph_slot->bitmap.width + 1;
  }

  return true;
}
