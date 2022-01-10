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

#pragma once
#include "Camera.hpp"
#include "Viewport.hpp"
#include <cstdint>
#include <utility>
#include <array>

namespace ASGE
{
  class Resolution
  {
   public:
    enum class Policy : uint8_t
    {
      NONE     = 0, /**< Does not apply any offset or scaling to the game */
      MAINTAIN = 1, /**< Maintains the aspect ratio and centers where possible */
      CENTER   = 2, /**< Centers but does not scale the viewport */
      SCALE    = 3, /**< Fills the screen which may lead to stretching or distortion */
      ZOOM     = 4, /**< Maintains the aspect ratio but fills the screens */
    };

   public:
    std::array<int32_t, 2> base{ 1920, 1080 };
    std::array<int32_t, 2> window{ 1920, 1080 };
    std::array<int32_t, 3> desktop{ 0, 0, 0 };
    ASGE::Viewport viewport{ 0, 0, 1920, 1080 };
    Camera::CameraView view{0, 0, 1920, 1080};

   public:
    Resolution()  = default;
    ~Resolution() = default;

    float getBaseAspectRatio();
    float getWindowAspectRatio();
    float getDesktopAspectRatio();
  };
}