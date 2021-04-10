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

/**
 * @file
 * @brief Struct @ref ASGE::SpriteBounds
 */

#ifndef ASGE_SPRITEBOUNDS_HPP
#define ASGE_SPRITEBOUNDS_HPP
#include "Point2D.hpp"

namespace ASGE
{
  /**
  * @brief Four vertices defining a sprites bound.
  *
  * Used to conveniently store 4 points together. These 4 points
  * typically define the for vertices defining the bounding rectangle
  * of a sprite. Note:There is no guarantee that the bounds stored here
  * are axis-aligned or ordered.
  */
  struct SpriteBounds
  {
    Point2D v1 = { 0, 0 }; /**< The first vertex position.   */
    Point2D v2 = { 0, 0 }; /**< The second vertex position   */
    Point2D v3 = { 0, 0 }; /**< The third vertex position.   */
    Point2D v4 = { 0, 0 }; /**< The fourth vertex position.  */
  };

  using TextBounds = SpriteBounds;
}  // namespace ASGE

#endif // ASGE_SPRITEBOUNDS_HPP
