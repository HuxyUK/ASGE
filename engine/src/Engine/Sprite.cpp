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
#include "Sprite.hpp"
#include <cmath>

float ASGE::Sprite::xPos() const noexcept
{
  return position[0];
}

void ASGE::Sprite::xPos(float x) noexcept
{
  position[0] = x;
}

float ASGE::Sprite::yPos() const noexcept
{
  return position[1];
}

void ASGE::Sprite::yPos(float y) noexcept
{
  position[1] = y;
}

float ASGE::Sprite::width() const noexcept
{
  return dims[0];
}

void ASGE::Sprite::width(float width) noexcept
{
  dims[0] = width;
}

float ASGE::Sprite::height() const noexcept
{
  return dims[1];
}

void ASGE::Sprite::height(float height) noexcept
{
  dims[1] = height;
}

void ASGE::Sprite::dimensions(float& width, float& height) const noexcept
{
  width  = dims[0];
  height = dims[1];
}

float ASGE::Sprite::rotationInRadians() const noexcept
{
  return angle;
}

float ASGE::Sprite::scale() const noexcept
{
  return scale_factor;
}

void ASGE::Sprite::rotationInRadians(float rotation_radians)
{
  angle = rotation_radians;
}

void ASGE::Sprite::scale(float scale_value) noexcept
{
  scale_factor = scale_value;
}

ASGE::Colour ASGE::Sprite::colour() const noexcept
{
  return tint;
}

void ASGE::Sprite::colour(ASGE::Colour sprite_colour) noexcept
{
  tint = sprite_colour;
}

bool ASGE::Sprite::isFlippedOnX() const noexcept
{
  return (flip_flags & FLIP_X) == FLIP_X;
}

bool ASGE::Sprite::isFlippedOnY() const noexcept
{
  return (flip_flags & FLIP_Y) == FLIP_Y;
}

bool ASGE::Sprite::isFlippedOnXY() const noexcept
{
  return (flip_flags & FLIP_XY) == FLIP_XY;
}

void ASGE::Sprite::setFlipFlags(FlipFlags flip) noexcept
{
  flip_flags = flip;
}

void ASGE::Sprite::opacity(float a) noexcept
{
  this->alpha = a;
}

float ASGE::Sprite::opacity() const noexcept
{
  return alpha;
}

float* ASGE::Sprite::srcRect() noexcept
{
  return &src_rect[0];
}

const float* ASGE::Sprite::srcRect() const noexcept
{
  return &src_rect[0];
}

ASGE::SpriteBounds ASGE::Sprite::getLocalBounds() const noexcept
{
  // clang-format off
  SpriteBounds bounds;
  bounds.v1 = {0,       0};
  bounds.v2 = {width(), 0};
  bounds.v3 = {width(), height()};
  bounds.v4 = {0,       height()};

  // clang-format on
  return bounds;
}

ASGE::SpriteBounds ASGE::Sprite::getWorldBounds() const noexcept
{
  SpriteBounds bounds;

  //   X = x*cos(θ) - y*sin(θ)
  //   Y = x*sin(θ) + y*cos(θ)
  auto theta = this->rotationInRadians();
  auto s     = sinf(theta);
  auto c     = cosf(theta);

  auto rotate = [&](float x, float y) {
    auto cx = x + (width() * scale()) * 0.5F;
    auto cy = y + (height() * scale()) * 0.5F;

    float x1 = x - cx;
    float y1 = y - cy;

    float x2 = x1 * c - y1 * s;
    float y2 = x1 * s + y1 * c;

    Point2D point;
    point.x = x2 + cx;
    point.y = y2 + cy;

    return point;
  };

  // TODO optimise this..
  bounds.v1 = rotate(this->xPos(), this->yPos());
  bounds.v2 = rotate(this->xPos() + width() * scale(), this->yPos());
  bounds.v3 = rotate(this->xPos() + width() * scale(), this->yPos() + height() * scale());
  bounds.v4 = rotate(this->xPos(), this->yPos() + height() * scale());
  return bounds;
}

ASGE::Sprite::FlipFlags ASGE::Sprite::flipFlags() const noexcept
{
  return flip_flags;
}

int16_t ASGE::Sprite::getGlobalZOrder() const noexcept
{
  return z_order;
}

void ASGE::Sprite::setGlobalZOrder(int16_t new_z_order) noexcept
{
  z_order = new_z_order;
}

const ASGE::SHADER_LIB::Shader* ASGE::Sprite::getPixelShader() const noexcept
{
  return this->shader;
}

ASGE::SHADER_LIB::Shader* ASGE::Sprite::getPixelShader() noexcept
{
  return this->shader;
}

void ASGE::Sprite::setPixelShader(ASGE::SHADER_LIB::Shader* shader_in) noexcept
{
  shader = shader_in;
}

bool ASGE::Sprite::hasPixelShader() const noexcept
{
  return shader != nullptr;
}

void ASGE::Sprite::setMagFilter(ASGE::Texture2D::MagFilter requested_filter) const noexcept
{
  if (getTexture() == nullptr)
  {
    Logging::WARN(__PRETTY_FUNCTION__);
    Logging::WARN("You need to initialise the texture before updating its sample settings");
    return;
  }

  getTexture()->updateMagFilter(requested_filter);
}

std::array<float, 2>& ASGE::Sprite::dimensions()
{
  return dims;
}

std::tuple<int, int> ASGE::Sprite::dimensions() const noexcept
{
  return std::make_tuple(static_cast<int>(width()), static_cast<int>(height()));
}

bool ASGE::Sprite::loadTexture(const std::string& id)
{
  return loadTexture(id, AttachMode(AttachMode::DEFAULT));
}

bool ASGE::Sprite::attach(ASGE::Texture2D* texture2D) noexcept
{
  return attach(texture2D, AttachMode(AttachMode::DEFAULT));
}

ASGE::Point2D ASGE::Sprite::midpoint() const
{
  return {
    xPos() + width() * scale_factor * 0.5F,
    yPos() + height() * scale_factor * 0.5F
  };
}
