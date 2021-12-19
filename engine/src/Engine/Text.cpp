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

#include "Logger.hpp"
#include "Text.hpp"
#include <utility>
ASGE::Text::Text(const ASGE::Font& font) : font(&font) {}

ASGE::Text::Text(const ASGE::Font& font, std::string string) :
  string(std::move(string)), font(&font)
{
}

ASGE::Text::Text(const ASGE::Font& font, const std::string& string, int x, int y) :
  Text(font, string)
{
  this->position = Point2D{ static_cast<float>(x), static_cast<float>(y) };
}

ASGE::Text::Text(const ASGE::Font& font, std::string&& string, int x, int y) :
  Text(font, std::move(string))
{
  this->position = Point2D{ static_cast<float>(x), static_cast<float>(y) };
}

ASGE::Text::Text(
  const ASGE::Font& font, const std::string& string, int x, int y, const ASGE::Colour& colour) :
  Text(font, string, x, y)
{
  this->colour = colour;
}

ASGE::Text::Text(
  const ASGE::Font& font, std::string&& string, int x, int y, const ASGE::Colour& colour) :
  Text(font, std::move(string), x, y)
{
  this->colour = colour;
}

int16_t ASGE::Text::getZOrder() const noexcept
{
  return this->z_order;
}

float ASGE::Text::getScale() const noexcept
{
  return this->scale;
}

float ASGE::Text::getOpacity() const noexcept
{
  return this->opacity;
}

const ASGE::Point2D& ASGE::Text::getPosition() const noexcept
{
  return this->position;
}

const ASGE::Colour& ASGE::Text::getColour() const noexcept
{
  return this->colour;
}

const std::string& ASGE::Text::getString() const noexcept
{
  return this->string;
}

const ASGE::Font& ASGE::Text::getFont() const noexcept
{
  return *this->font;
}

ASGE::Text& ASGE::Text::setString(const std::string& string) noexcept
{
  this->string = string;
  return *this;
}

ASGE::Text& ASGE::Text::setString(std::string&& string) noexcept
{
  this->string = std::move(string);
  return *this;
}

ASGE::Text& ASGE::Text::setColour(const ASGE::Colour& colour) noexcept
{
  this->colour = colour;
  return *this;
}

ASGE::Text& ASGE::Text::setFont(const ASGE::Font& font) noexcept
{
  this->font = &font;
  return *this;
}

ASGE::Text& ASGE::Text::setPositionY(float y) noexcept
{
  this->position.y = y;
  return *this;
}

ASGE::Text& ASGE::Text::setScale(float scale) noexcept
{
  this->scale = scale;
  return *this;
}

ASGE::Text& ASGE::Text::setZOrder(int16_t z_order) noexcept
{
  this->z_order = z_order;
  return *this;
}

ASGE::Text& ASGE::Text::setOpacity(float opacity) noexcept
{
  this->opacity = opacity;
  return *this;
}

ASGE::Text& ASGE::Text::setPositionX(float x) noexcept
{
  this->position.x = x;
  return *this;
}

ASGE::Text& ASGE::Text::setPosition(const ASGE::Point2D& position) noexcept
{
  this->position = position;
  return *this;
}

ASGE::Text& ASGE::Text::setPosition(ASGE::Point2D&& position) noexcept
{
  this->position = std::move(position);
  return *this;
}

int ASGE::Text::getLineSpacing() const
{
  if (validFont())
  {
    return static_cast<int>(font->line_height * scale);
  }

  return 0;
}

bool ASGE::Text::validFont() const noexcept
{
  if (font == nullptr)
  {
    Logging::WARN("ASGE::Text does not have a valid font set");
    return false;
  }
  return true;
}

ASGE::TextBounds ASGE::Text::getLocalBounds() const
{
  TextBounds bounds;
  if (validFont())
  {
    auto width     = getWidth();
    auto [min,max] = font->boundsY(string, scale);

    // clang-format off
    bounds.v1 = {0,     0      };
    bounds.v2 = {width, 0      };
    bounds.v3 = {width, min+max};
    bounds.v4 = {0,     min+max};
    // clang-format on
  }
  return bounds;
}

ASGE::TextBounds ASGE::Text::getWorldBounds() const
{
  TextBounds bounds;
  if (validFont())
  {
    auto width      = getWidth();
    auto [min, max] = font->boundsY(string, scale);

    // clang-format off
    bounds.v1 = {this->position.x,         this->position.y - min};
    bounds.v2 = {this->position.x + width, this->position.y - min};
    bounds.v3 = {this->position.x + width, this->position.y + max};
    bounds.v4 = {this->position.x,         this->position.y + max};
    // clang-format on
  }
  return bounds;
}

ASGE::Text::Text(ASGE::Text&& rhs) noexcept :
  string(std::move(rhs.string)),
  colour(rhs.colour),
  position(std::move(rhs.position)),
  opacity(rhs.opacity),
  scale(rhs.scale),
  z_order(rhs.z_order),
  font(rhs.font)
{
  rhs.font = nullptr;
}

ASGE::Text& ASGE::Text::operator=(ASGE::Text&& rhs) noexcept
{
  this->string   = std::move(rhs.string);
  this->colour   = rhs.colour;
  this->position = std::move(rhs.position);
  this->opacity  = rhs.opacity;
  this->scale    = rhs.scale;
  this->z_order  = rhs.z_order;
  this->font     = rhs.font;
  rhs.font       = nullptr;

  return *this;
}

float ASGE::Text::getWidth() const noexcept
{
  if (validFont())
  {
    return font->pxWide(string, scale);
  }

  return 0;
}

float ASGE::Text::getHeight() const noexcept
{
  if (validFont())
  {
    return font->pxHeight(string, scale);
  }

  return 0;
}

