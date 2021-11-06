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

//! @file GameTime.hpp
//! @brief Struct @ref ASGE::GameTime

#pragma once
#include <chrono>
namespace ASGE
{
  /**
   *  @brief Stores both frame and game deltas.
   *
   *  Stores various measurements of delta time for the
   *  game. It will store the current time point which
   *  is used to calculate the delta between frames, the
   *  delta of the last frame and the entire delta since the
   *  game started. This can be used to control simulations
   *  and render functions to use non-fixed time steps.
   */
  struct GameTime
  {
    /**
     *  Time point of last rendered frame.
     *  The time at which the previous frame render began.
     */
    std::chrono::time_point<std::chrono::steady_clock> last_frame_time =
      std::chrono::steady_clock::now();

    /**
     *  Time point of last fixed step update.
     *  The time at which the previous update tick began.
     */
    std::chrono::time_point<std::chrono::steady_clock> last_fixedstep_time =
      std::chrono::steady_clock::now();

    /**
     *  Frame delta.
     *  How long did it take between the previous frame being rendered and the
     *  current one starting. This can be used to help interpolate objects
     *  using the render (variable time-step) function.
     */
    std::chrono::duration<double, std::milli> frame_delta;

    /**
     *  @brief Fixed delta time.
     *
     *  The delta between each fixed time-step. Use this to create
     *  deterministic simulations inside the update function.
     *
     *  @Note This does not reflect the delta in real time between
     *  updates, rather it represents the simulated amount of time
     *  to aim for between them.
     */
    std::chrono::duration<double, std::milli> fixed_delta;

    /**
     *  @brief Distance to next fixed update
     *
     *  Used in combination with fixed time-steps. It represents how far
     *  far along (percentage) to the next fixed time step we are. An update
     *  call will only happen once this distance is 1.0 or higher. To
     *  re-frame, how much simulated time has not yet been processed.
     */
    double distance = 0.0;

    /**
     *  @brief Total running time.
     *
     *  The total amount of time since the start of the game in milliseconds.
     *  The longer the game runs the higher this value will be. Could be
     *  expanded upon to support pausing by storing the time the game is in
     *  a paused state. However, currently the paused data is not exposed.
     */
    std::chrono::milliseconds elapsed;

    /**
     * @brief The last frame delta expressed in seconds (double)
     *
     * Just a shorthand function to convert the chrono::duration of
     * frame_delta in seconds to help with pacing of animations etc. This
     * is purely just to present it in a more user friendly number.
     *
     * @return The last frame delta in seconds.
     */
    [[nodiscard]] double deltaInSecs() const noexcept
    {
      using namespace std::chrono_literals;
      return frame_delta.count() / std::chrono::duration_cast<std::chrono::milliseconds>(1s).count();
    }

    /**
     * @brief The fixed delta expressed in seconds (double)
     *
     * Just a shorthand function to convert the chrono::duration of
     * fixed_delta in seconds to help with simulations and fixed time-step
     * update functions. It is purely just to present it in a more user
     * friendly number.
     *
     * @return The fixed time-step in seconds.
     */
    [[nodiscard]] double fixedTsInSecs() const noexcept
    {
      using namespace std::chrono_literals;
      return fixed_delta.count() / std::chrono::duration_cast<std::chrono::milliseconds>(1s).count();
    }
  };
} // namespace ASGE
