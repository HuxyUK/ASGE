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

#include <cmath>
#include "Point2D.hpp"

ASGE::Point2D::Point2D(float x, float y) : x(x), y(y) {}

float ASGE::Point2D::distance(const Point2D& rhs) const
{
  return sqrtf(powf(this->x - rhs.x, 2) + powf(this->y - rhs.y, 2));
}

ASGE::Point2D ASGE::Point2D::midpoint(const Point2D& rhs) const
{
  Point2D mid;
  mid.x = (this->x + rhs.x) * 0.5F;
  mid.y = (this->y + rhs.y) * 0.5F;
  return mid;
}

ASGE::Point2D& ASGE::Point2D::operator=(const Point2D& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }

  this->x = rhs.x;
  this->y = rhs.y;
  return *this;
}

ASGE::Point2D& ASGE::Point2D::operator+=(const Point2D& rhs)
{
  this->x += rhs.x;
  this->y += rhs.y;
  return *this;
}

ASGE::Point2D& ASGE::Point2D::operator*=(float scalar)
{
  this->x *= scalar;
  this->y *= scalar;
  return *this;
}

ASGE::Point2D ASGE::Point2D::operator*(float scalar) const
{
  return ASGE::Point2D{ this->x * scalar, this->y * scalar };
}

ASGE::Point2D& ASGE::Point2D::operator=(ASGE::Point2D&& rhs) noexcept
{
  this->x = rhs.x;
  this->y = rhs.y;
  return *this;
}

ASGE::Point2D::Point2D(ASGE::Point2D&& rhs) noexcept : x(rhs.x), y(rhs.y) {}

std::string ASGE::Point2D::toString() const
{
  return "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
}

ASGE::Point2D ASGE::operator*(float f, const ASGE::Point2D& v)
{
  return {f * v.x, f * v.y};
}

bool ASGE::Point2D::operator==(const ASGE::Point2D& rhs) const
{
  return (this->x == rhs.x && this->y == rhs.y);
}

bool ASGE::Point2D::operator!=(const ASGE::Point2D& rhs) const
{
  return (this->x != rhs.x || this->y == rhs.y);
}
