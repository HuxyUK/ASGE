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

//! @file Viewport.hpp
//! @brief Struct @ref ASGE::Viewport

#ifndef ASGE_VIEWPORT_HPP
#define ASGE_VIEWPORT_HPP

#include <cstdint>
namespace ASGE
{
  /**
   * A viewport controls the mapping of the rendered scene on to
   * the destination target. The destination could be a ASGE::RenderTarget
   * or the game window itself. For example you could create a viewport that
   * only consumed half the render  window and render a camera's view in to
   * it. This could allow for more complex game scenes such as the use of
   * split screens.
   *
   * In the following example, the game screen is split into half
   * (vertically) with one viewport on the left and the other on the
   * right. A camera is then used to frame the action, before rendering
   * the first sprite, shifting the viewport and then rendering the
   * second sprite.
   * @image html "viewports.png" "split rendering" width=300
   *
   * @note
   *  Changing the size of the viewport will directly impact how the rendered
   *  sprites are mapped to the screen. This can result in distortion when
   *  your viewport has a different aspect ratio compared to the camera.
   * @image html "viewports2.png" "poor aspect ratio split rendering" width=600
   */
  struct Viewport
  {
    /**
     * Default constructor.
     */
    Viewport() = default;

    /**
     * The constructor for a viewport, which uses the provided dimensions.
     * @param[in] xpos The position the X axis that the viewport should start.
     * @param[in] ypos The position the Y axis that the viewport should start.
     * @param[in] width The width of the viewport.
     * @param[in] height The height of the viewport.
     */
    Viewport(int32_t xpos, int32_t ypos, int32_t width, int32_t height) :
      x(xpos), y(ypos), w(width), h(height)
    {

    }

    friend bool operator!=(const Viewport& rhs, const Viewport& lhs)
    {
      return lhs.x != rhs.x || lhs.y != rhs.y ||
             rhs.w != lhs.w || lhs.h != rhs.h;
    }

    int32_t x = 0;  /**< The starting X position */
    int32_t y = 0;  /**< The starting Y position */
    int32_t w = 0; /**< The width of the viewport */
    int32_t h = 0; /**< The height of the viewport */
  };
}  // namespace ASGE

#endif // ASGE_VIEWPORT_HPP
