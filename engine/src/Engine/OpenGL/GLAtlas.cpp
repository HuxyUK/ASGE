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

constexpr int TEXTURE_WIDTH = 1024;

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H
#include <msdfgen.h>
#include <msdfgen-ext.h>

// Std Library
#include <iostream>
#include <utility>

// Engine related
#include "Logger.hpp"
#include "Point2D.hpp"

namespace
{
  struct msdf_char
  {
    msdf_char(unsigned long scan_, ASGE::Point2D xy_, int width, int height) :
      scan_code(scan_), xy(std::move(xy_)), bitmap(width, height)
    {
    }

    ASGE::Point2D xy{ 0, 0 };
    msdfgen::Bitmap<float, 3> bitmap;
    unsigned long scan_code{ 0x00 };
  };

  struct pixel
  {
    std::byte r{ 0 };
    std::byte g{ 0 };
    std::byte b{ 0 };
    std::byte a{ 0 };
  };
} // namespace

ASGE::FontTextureAtlas::~FontTextureAtlas()
{
  if (glfwGetCurrentContext() != nullptr)
  {
    glDeleteTextures(1, &texture);
  }
}

//TODO: improve atlas generation and performance
bool ASGE::FontTextureAtlas::init(
  const FT_Face& face, msdfgen::FontHandle* font_handle,
  double glyph_size,
  double range, double font_scale)
{
  const float PADDING = 1;
  std::vector<msdf_char> msdfs;
  msdfs.reserve(face->num_glyphs);

  double geometry_scale = font_scale / (face->units_per_EM >> 6);
  range = (range / glyph_size) / geometry_scale;
  glyph_size *= geometry_scale;

  int32_t glyph_height = 0;
  int32_t glyph_width  = 0;
  int32_t pos_x        = 0;
  int32_t row_height   = 0;

  FT_UInt gindex    = 0;
  FT_ULong charcode = FT_Get_First_Char(face, &gindex);
  while (gindex > 0 && charcode < 128)
  {
    msdfgen::Shape shape;
    msdfgen::Shape::Bounds bounds{};
    double advance = 0;

    if (msdfgen::loadGlyph(shape, font_handle, charcode, &advance))
    {
      auto& ch     = characters[charcode];
      ch.Advance.x = int(advance * glyph_size);

      if (shape.validate() && !shape.contours.empty())
      {
        shape.normalize();
        shape.inverseYAxis = true;

        bounds = shape.getBounds(range, 1);

        double w     = glyph_size * (bounds.r - bounds.l);
        double h     = glyph_size * (bounds.t - bounds.b);
        glyph_width  = ceil(bounds.r - bounds.l) * glyph_size;
        glyph_height = ceil(bounds.t - bounds.b) * glyph_size;

        double translate_x = -bounds.l + (glyph_width - w) / glyph_size;
        double translate_y = -bounds.b + (glyph_height - h) / glyph_size;

        if (pos_x + glyph_width > TEXTURE_WIDTH)
        {
          width = std::max(width, pos_x);
          height += row_height + PADDING;
          pos_x = row_height = 0;
        }

        ch.Size    = { static_cast<float>(glyph_width), static_cast<float>(glyph_height) };
        ch.Bearing.x = { static_cast<int>(bounds.l * glyph_size) };
        ch.Bearing.y = { static_cast<int>(bounds.t * glyph_size) };
        msdfgen::edgeColoringInkTrap(shape, 3.0);

        auto& msdf = msdfs.emplace_back(msdf_char(
          charcode,
          { static_cast<float>(pos_x), static_cast<float>(height) },
          glyph_width,
          glyph_height));

        msdfgen::generateMSDF(
          msdf.bitmap,
          shape,
          range,
          msdfgen::Vector2(glyph_size, glyph_size),
          msdfgen::Vector2(translate_x, translate_y));

        pos_x += glyph_width + PADDING;
        row_height = std::max(row_height, glyph_height);
      }
    }
    charcode = FT_Get_Next_Char(face, charcode, &gindex);
  }

  // set the texture's width and height
  width = std::max(width, pos_x);
  height += row_height;

  std::vector<pixel> pixels;
  pixels.reserve(width * height);
  pixels.assign(width * height, pixel());
  for (auto& msdf : msdfs)
  {
    auto& ch = characters[msdf.scan_code];
    ch.UV[0] = msdf.xy.x / (float)width;
    ch.UV[1] = msdf.xy.y / (float)height;
    ch.UV[2] = ch.UV[0] + msdf.bitmap.width() / (float)width;
    ch.UV[3] = ch.UV[1] + msdf.bitmap.height() / (float)height;

    auto idx = (msdf.xy.y * width) + msdf.xy.x;
    for (auto row = 0; row < msdf.bitmap.height(); ++row)
    {
      for (auto col = 0; col < msdf.bitmap.width(); ++col)
      {
        auto& pixel = pixels[idx++];
        pixel.r     = static_cast<std::byte>(msdfgen::pixelFloatToByte(msdf.bitmap(col, row)[0]));
        pixel.g     = static_cast<std::byte>(msdfgen::pixelFloatToByte(msdf.bitmap(col, row)[1]));
        pixel.b     = static_cast<std::byte>(msdfgen::pixelFloatToByte(msdf.bitmap(col, row)[2]));
        pixel.a     = static_cast<std::byte>(255);
      }

      // next row + rewind to beginning of bitmap x
      idx += width - msdf.bitmap.width();
    }
  }

  allocateTexture(pixels.data());
  glBindTexture(GL_TEXTURE_2D, 0);

  Logging::DEBUG(std::string("Generated Font Atlas: ").append(face->family_name));
  std::stringstream ss;
  ss << "Generated a " << width << "x " << height << " (" << width * height / 1024<< " kb) texture atlas";
  Logging::DEBUG(ss.str());
  return true;
}

GLuint ASGE::FontTextureAtlas::getTextureID() const noexcept
{
  return texture;
}

const ASGE::Character& ASGE::FontTextureAtlas::getCharacter(int idx) const
{
  return characters.at(idx);
}

void ASGE::FontTextureAtlas::allocateTexture(const void* data)
{
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  setSampleParams();
  ASGE::ClearGLErrors("Error allocating texture for font atlas");
}

void ASGE::FontTextureAtlas::setSampleParams()
{
  GLVMSG(__PRETTY_FUNCTION__, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GLVMSG(__PRETTY_FUNCTION__, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GLVMSG(__PRETTY_FUNCTION__, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  GLVMSG(__PRETTY_FUNCTION__, glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
