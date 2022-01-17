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

#include "GLFontSet.hpp"
#include "GLAtlas.hpp"
#include "GLTexture.hpp"
#include <vector>

ASGE::GLFontSet::GLFontSet(GLFontSet&& rhs) noexcept : atlas(std::move(rhs.atlas))
{
  font_size   = rhs.font_size;
  font_name   = rhs.font_name;
  line_height = rhs.line_height;
  px_range    = rhs.px_range;
}

ASGE::GLFontSet::~GLFontSet()
{
  font_size = 0;
}

const ASGE::FontTextureAtlas* ASGE::GLFontSet::getAtlas() const noexcept
{
  return atlas.get();
}

float ASGE::GLFontSet::pxWide(char ch, float scale) const noexcept
{
  const auto* atlas_ch = &atlas->getCharacter(ch);
  return atlas_ch->Advance.x * scale;
}

float ASGE::GLFontSet::pxWide(const std::string& string, float scale) const
{
  std::size_t found = string.find_first_not_of('\n');
  if (found == std::string::npos)
  {
    return 0;
  }

  const Character* ch = nullptr;

  float length    = 0;
  float max_width = 0;

  auto update_length = [&]()
  {
    length -= float(ch->Advance.x - ch->Size.x) * scale;
    if (length > max_width)
    {
      max_width = length;
    }
    length = 0;
  };

  for (const char C : string)
  {
    if (C == '\n')
    {
      update_length();
      continue;
    }

    ch = &atlas->getCharacter(C);
    length += ch->Advance.x * scale;
  }

  update_length();
  return max_width;
}

void ASGE::GLFontSet::setAtlas(ASGE::FontTextureAtlas* atlas_) noexcept
{
  this->atlas.reset(atlas_);
}

float ASGE::GLFontSet::pxHeight(const std::string& string, float scale) const
{
  if (string.empty())
  {
    return 0;
  }

  int height            = 0;
  std::string delimiter = "\n";
  for (const auto& c : string)
  {
    if (std::to_string(c) == delimiter)
    {
      break;
    }

    const auto* ch = &atlas->getCharacter(c);
    height         = std::max(height, ch->Bearing.y);
  }

  return static_cast<float>(height) * scale;
}

ASGE::GLFontSet& ASGE::GLFontSet::operator=(ASGE::GLFontSet&& rhs) noexcept
{
  this->font_size   = rhs.font_size;
  this->font_name   = rhs.font_name;
  this->line_height = rhs.line_height;
  this->atlas       = std::move(rhs.atlas);
  return *this;
}

std::tuple<float, float> ASGE::GLFontSet::boundsY(const std::string& string, float scale) const
{
  if (string.empty())
  {
    return std::make_tuple(0, 0);
  }

  std::tuple<float, float> bounds(0, 0);
  auto& [min_y, max_y] = bounds;

  std::string delimiter       = "\n";
  std::string::size_type prev = 0;
  std::string::size_type pos  = 0;
  std::vector<std::string> lines;

  while ((pos = string.find(delimiter, prev)) != std::string::npos)
  {
    lines.emplace_back(string.substr(prev, pos));
    prev = pos + delimiter.size();
  }

  lines.emplace_back(string.substr(prev, pos));

  // first line tells us the low y value
  for (const auto& c : lines.front())
  {
    const auto* ch = &atlas->getCharacter(c);
    min_y = std::max(min_y, static_cast<float>(ch->Bearing.y));
  }

  // last line tells us the high y value
  for (const auto& c : lines.back())
  {
    const auto* ch = &atlas->getCharacter(c);
    max_y          = std::max(max_y, static_cast<float>(ch->Size.y - ch->Bearing.y));
  }

  float line_count = static_cast<float>(lines.size()) - 1;
  max_y = (max_y + (line_count * line_height)) * scale;
  min_y *= scale;
  return bounds;
}

void ASGE::GLFontSet::setMagFilter(ASGE::Texture2D::MagFilter mag_filter)
{
  if (atlas)
  {
    glBindTexture(GL_TEXTURE_2D, atlas->getTextureID());
    GLVMSG(
      __PRETTY_FUNCTION__,
      glTexParameteri, GL_TEXTURE_2D,
      GL_TEXTURE_MAG_FILTER, GLTexture::GL_MAG_LOOKUP.at(mag_filter));
  }
}