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

ASGE::GLFontSet::GLFontSet(GLFontSet&& rhs) noexcept : atlas(std::move(rhs.atlas))
{
  font_size   = rhs.font_size;
  font_name   = rhs.font_name;
  line_height = rhs.line_height;
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
  return atlas_ch->Advance.x * powf(scale, 2);
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

  auto update_length = [&]() {
    length -= float(ch->Advance.x - ch->Size.x) * powf(scale, 2);
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
    length += ch->Advance.x * powf(scale, 2);
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

  float height               = 0;
  std::string::size_type pos = 0;
  std::string target         = "\n";
  while ((pos = string.find(target, pos)) != std::string::npos)
  {
    height += (float)line_height * scale;
    pos += target.length();
  }

  return height;
}

ASGE::GLFontSet& ASGE::GLFontSet::operator=(ASGE::GLFontSet&& rhs) noexcept
{
  this->font_size   = rhs.font_size;
  this->font_name   = rhs.font_name;
  this->line_height = rhs.line_height;
  this->atlas       = std::move(rhs.atlas);
  return *this;
}
