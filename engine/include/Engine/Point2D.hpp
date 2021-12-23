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

//! @file Point2D.hpp
//! @brief Class @ref ASGE::Point2D

#ifndef ASGE_POINT_H
#define ASGE_POINT_H

#include <array>
#include <string>

namespace ASGE
{
  /**
* @brief A point in 2D space.
*
* Used to represent a position in 2D space. Includes some basic
* helper functionality for distance and mid-point calculations.
   */
  struct Point2D
  {
    float x = 0; /**< the x position.   */
    float y = 0; /**< the y position.   */
    ~Point2D() = default;

    /**
    * Default constructor.
    * The constructor takes will default the position to 0,0
    * unless the x or x and y parameters are provided. These
    * are then mapped on the x,y values inside the struct.
    * @param x The x coordinate.
    * @param y The y coordinate.
    */
    //NOLINTNEXTLINE(hicpp-explicit-conversions)
    Point2D(float x = 0, float y = 0);

    /**
    * Assigns the value of another point2D to this one.
    * Assignment constructor, that copies each field from
    * rhs in to the lhs.
    * @param Point2D.
    */
    Point2D& operator=(const Point2D&);

    /**
    * Midpoint.
    * Calculates the midpoint between two different points.
    * @param rhs The second point to use.
    * @return The midpoint as a Point2D.
    */
    [[nodiscard]] Point2D midpoint(const Point2D& rhs) const;

    /**
    * Distance.
    * Given two points, calculate the distance between them.
    * @param The second point to use.
    * @return The distance between the two points.
    */
    [[nodiscard]] float distance(const Point2D& rhs) const;

    Point2D& operator+=(const Point2D& rhs);
    Point2D& operator*=(float scalar);
    Point2D  operator*(float scalar) const;

    Point2D(const Point2D&) = default;
    Point2D(Point2D&&) noexcept;
    Point2D& operator=(Point2D&&) noexcept;
    bool operator== (const Point2D& rhs) const;
    bool operator!= (const Point2D& rhs) const;

    /**
     * Converts to string.
     * Human-friendly version of the Point2D
     * @return The string representation of the Point2D class
     */
    [[nodiscard]] std::string toString() const;
  };


  /*
   * Multiply float by Point2D.
   * @return the resultant Point2D
   */
  Point2D operator*(float f, const Point2D &v);

}  // namespace ASGE




#endif //ASGE_POINT_H
