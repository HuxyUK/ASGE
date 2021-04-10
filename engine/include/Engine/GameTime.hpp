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
namespace ASGE {

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
     *  Time point of last tick.
     *  The time at which the previous tick update began.
     */
    std::chrono::time_point<std::chrono::high_resolution_clock> frame_time = std::chrono::high_resolution_clock::now();
    using delta_time = decltype(frame_time);

		/**
		 *  Tick delta.
		 *  The amount of elapsed time since the last update.
		 */
		std::chrono::duration<double, std::milli> frame_delta;

    /**
     *  Fixed delta time.
     *  The amount of delta between fixed updates.
     */
    std::chrono::duration<double, std::milli> fixed_delta;

    /**
     *  Distance to next fixed update
     *  How far along (percentage) to the next fixed time step are we?
     *  Or to re-frame, how much time has not yet been simulated?
     */
    double distance;

		/**
		 *  Running time.
		 *  The amount of time since the start of the game.
		 */
		std::chrono::milliseconds elapsed;

		/// The last delta expressed in seconds (double)
		/// Just a shorthand function to convert the chrono::duration
		/// into a more user friendly number.
		/// \return The last frame delta in seconds
    [[nodiscard]] double deltaInSecs() const noexcept
    {
      using namespace std::chrono_literals;
      return frame_delta.count() /
             std::chrono::duration_cast<std::chrono::milliseconds>(1s).count();
    }
  };
}  // namespace ASGE
