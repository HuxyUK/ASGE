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

//! @file Camera.hpp
//! @brief Class @ref ASGE::Camera, Struct @ref ASGE::Camera::CameraView

#ifndef ASGE_CAMERA_HPP
#define ASGE_CAMERA_HPP

#include "GameTime.hpp"
#include "Point2D.hpp"
#include "Viewport.hpp"
#include <array>

namespace ASGE
{
  /**
   *  @brief A 2D orthogonal camera class.
   *
   *  The Camera class is designed as a simple way to alter the view
   *  being drawn to the screen. The view described by the camera
   *  will be scaled to the viewport, so it is recommend to size it
   *  identically to the viewport or to at least use the same aspect
   *  ratio to prevent scaling artifacts. This basic implementation
   *  provides translation in the X and Y axis and use of Z to control
   *  the zoom level of the camera. In order to apply a translation
   *  to the camera, the translate function must be called. The default
   *  implementation is to move the camera by the requested translation
   *  amount.There is no damping or smoothing applied. If this is
   *  required, the class should be inherited from and the update
   *  function overridden.
   *
   *  @image html "http://www.pbr-book.org/3ed-2018/Camera_Models/Ortho%20generate%20ray.svg" "Physically Based Rendering: From Theory To Implementation, © 2004-2019 Matt Pharr, Wenzel Jakob, and Greg Humphreys" width=50%
   *
   *  <example>
   *  Usage:
   *  @code
   *    ASGE::Camera camera {1024, 768};
   *    camera.translateY(-2048 * gt.deltaInSecs());
   *    camera.update(gt);
   *    renderer->setProjectionMatrix(camera.getView());
   *  @endcode
   *  </example>
   */
  class Camera
  {
   public:
    using Translation = std::array<float, 3>;

    /**
     * @brief A an orthogonal camera view struct.
     *
     * The camera view class describes a bounding box that is used
     * to map the game world to the camera’s lens. Objects outside
     * of the the view will not be rendered. The camera view can be
     * applied to the renderer during the drawing phase of the game.
     * It’s possible to swap camera views during a render cycle.
     * The easiest way to generate the view is to use a Camera class,
     * assign it a width and height and update its focal point using
     */
    struct CameraView
    {
      float min_x = 0.0F; /**< The minimum x position to include */
      float min_y = 0.0F; /**< The minimum y position to include */
      float max_x = 0.0F; /**< The maximum x position to include */
      float max_y = 0.0F; /**< The maximum y position to include */
    };

   public:
    /**
     * Default constructor with no predefined view.
     * @note Not providing a view will result in nothing being rendered.
     */
    Camera() = default;

    /**
     * @brief Constructs the camera and sets the view width and height.
     *
     * Whilst the camera view's width and height will be set, it
     * assumed that the camera will be looking at {0,0}. This is
     * considered the center of the camera's view and is analogous to
     * its focal point. The final view will be sized accordingly i.e.
     * {-width, width} and {-height, height}.
     *
     * @param[in] width The width of the camera's view.
     * @param[in] height The height of the camera's view.
     */
    Camera(float width, float height);

    /**
     * @brief Constructs a camera, with a focal point and a view size.
     *
     * These parameters will dictate where the camera is looking
     * and how much of the scene it will show. It's important to note
     * that the focal point of a camera is the middle of it's view,
     * not the top left (0,0) i.e. {-width, width} and {-height, height}.
     *
     * @param[in] focal_point The camera's focal point.
     * @param[in] width: The width of the camera's view.
     * @param[in] height The height of the camera's view.
     */
    Camera(Point2D focal_point, float width, float height);

    /**
     * Default destructor.
     */
    virtual ~Camera() = default;


    /**
     * @brief Set's the camera's focal point.
     * @param focal_point The new center point of the camera's view.
     * @see ASGE::Point2D
     */
    void lookAt(Point2D focal_point);

    /**
     * @brief Does nothing.
     *
     * Override this function if you wish to add more complex
     * behaviours such as smoothing and delta-time related animations
     * to the camera's movement.
     *
     * @param[in] game_time The game's duration and the frame time delta.
     */
    virtual void update(const ASGE::GameTime& game_time);

    /**
     * @brief Adds a translation amount in all three axis.
     *
     * The translation is not applied until the update function is
     * called. You can think of the translation vector applied to
     * the camera's movement.
     *
     * @param[in] x The amount to move in the x axis.
     * @param[in] y The amount to move in the y axis.
     * @param[in] z The amount of zoom to apply.
     */
    virtual void translate(float x, float y, float z);

    /**
     * Moves the camera's view in the x axis.
     * @param[in] x The amount to translate the view by.
     */
    virtual void translateX(float x);

    /**
     * Moves the camera's view in the y axis.
     * @param[in] y The amount to translate the view by.
     */
    virtual void translateY(float y);

    /**
     * Adjust the camera's zoom.
     * @param[in] zoom The amount to adjust the zoom by.
     */
    virtual void translateZ(float zoom);

    /**
     * Adjust the camera's zoom.
     * @param[in] zoom The amount to adjust the zoom by.
     */
    void setZoom(float zoom);

    /**
     * Resizes the camera's view.
     * @note This does not maintain the aspect ratio of the camera.
     * @param[in] width The new width of the camera's view.
     * @param[in] height The new height of the camera's view.
     */
    void resize(float width, float height);

    /**
     * @brief A view that describes the camera's framing.
     *
     * Use this to frame the action, which in turn is rendered to the
     * window. This view can be used directly to control the
     * projection matrix used for mapping to the game window.
     * Everything inside the view will be mapped directly to the
     * screens viewport. The bounding volume controls the depth of
     * items rendered.
     *
     * @return ASGE::Camera::CameraView
     * @image html "http://www.pbr-book.org/3ed-2018/Camera_Models/Ortho%20viewing%20volume.svg" "Physically Based Rendering: From Theory To Implementation, © 2004-2019 Matt Pharr, Wenzel Jakob, and Greg Humphreys" width=25%
     */
    [[nodiscard]] CameraView getView() const;

    /**
     * Retrieves the current zoom level for the camera.
     * @return The camera's zoom.
     */
    [[nodiscard]] float getZoom() const;

    /**
     * Retrieves the camera's current position on the XY axis.
     * @return The camera's position.
     */
    [[nodiscard]] const ASGE::Point2D& position() const;

    /**
     * @brief Clamps the camera view.
     *
     * It is often desirable to clamp the field of vision for the camera to
     * ensure only specific regions of the game screen are shown. Consider a
     * tile map where the camera should not exceed the width or height of the
     * map. This function can be used to clamp the view to a set of predefined
     * values, representing `min_x`, `max_x`, `min_y` and `max_y`. These would
     * then be used to simply clamp the view's x and y positions.
     *
     * @param[in] view_bounds The four bounds to use in xy space.
     */
    void clamp(const CameraView&);

   private:
    ASGE::Point2D xy_pos{ 0, 0 };
    std::array<float, 2> dimensions{ 0, 0 };
    float zoom = 1.0F;
  };
} // namespace ASGE
#endif // ASGE_CAMERA_HPP
