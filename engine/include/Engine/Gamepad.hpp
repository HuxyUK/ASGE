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

//! @file Gamepad.hpp
//! @brief Struct @ref ASGE::GamePadData

#pragma once
#include "NonCopyable.hpp"
#include <algorithm>
#include <utility>
namespace ASGE
{
  /**
   * @brief Data for connected gamepad.
   *
   * Data that represents the current state of a connected
   * game pad. Includes the number of axis and buttons
   * along with their current state. If a gamepad is not
   * connected then the status of is_connected will be
   * false.
   *
   * @warning Do not work with non connected devices as it will lead to
   * undefined behaviour.
   *
   * @note
   * Currently, gamepad support is via polling and not events.
   *
   * <example>
   * Usage:
   * @code
   * 	auto gamepad = inputs->getGamePad();
   * 	if (gamepad.is_connected)
   * 	{
   *
   * 	}
   * @endcode
   * </example>
   */
  struct GamePadData
  {
    /**
        * Default constructor.
        * @param id The index of the controller.
        * @param gamepad_name The name of the controller.
        * @param axis_data The value of each axis.
        * @param button_state The state of each button.
     */
    GamePadData(
      int id, const char* gamepad_name,
      const float axis_data[6], const unsigned char button_state[15] ) noexcept :
      idx(id),
      name(gamepad_name)
    {
      if (axis_data)
        std::copy_n(axis_data, 6, this->axis);
      if (button_state)
        std::copy_n(button_state, 15, this->buttons);
    }

    GamePadData(GamePadData&& rhs) = default;
    GamePadData(const GamePadData& rhs)  = default;
    GamePadData& operator=(GamePadData&& rhs) = default;
    GamePadData& operator=(const GamePadData& rhs)  = default;
    ~GamePadData() = default;

    float axis[6]{ 0.0F }; /**< State of axis. The value of each axis  */               // NOLINT
    unsigned char buttons[15]{ 0 }; /**< State of buttons. The value of each button  */ // NOLINT
    const char* name{ nullptr }; /**< Name. The name of the connected controller */     // NOLINT
    int idx{ 0 }; /**< Index. The index for this controller */                          // NOLINT
    bool is_connected{ false }; /**< Is controller connected? */                        // NOLINT
  };

  namespace GAMEPAD
  {
    static constexpr int BUTTON_A = 0;
    static constexpr int BUTTON_B = 1;
    static constexpr int BUTTON_X = 2;
    static constexpr int BUTTON_Y = 3;
    static constexpr int BUTTON_LEFT_BUMPER = 4;
    static constexpr int BUTTON_RIGHT_BUMPER = 5;
    static constexpr int BUTTON_BACK = 6;
    static constexpr int BUTTON_START = 7;
    static constexpr int BUTTON_GUIDE = 8;
    static constexpr int BUTTON_LEFT_THUMB = 9;
    static constexpr int BUTTON_RIGHT_THUMB = 10;
    static constexpr int BUTTON_DPAD_UP = 11;
    static constexpr int BUTTON_DPAD_RIGHT = 12;
    static constexpr int BUTTON_DPAD_DOWN = 13;
    static constexpr int BUTTON_DPAD_LEFT = 14;
    static constexpr int BUTTON_LAST = BUTTON_DPAD_LEFT;
    static constexpr int BUTTON_CROSS = BUTTON_A;
    static constexpr int BUTTON_CIRCLE = BUTTON_B;
    static constexpr int BUTTON_SQUARE = BUTTON_X;
    static constexpr int BUTTON_TRIANGLE = BUTTON_Y;
    static constexpr int AXIS_LEFT_X = 0;
    static constexpr int AXIS_LEFT_Y = 1;
    static constexpr int AXIS_RIGHT_X = 2;
    static constexpr int AXIS_RIGHT_Y = 3;
    static constexpr int AXIS_LEFT_TRIGGER = 4;
    static constexpr int AXIS_RIGHT_TRIGGER = 5;
    static constexpr int AXIS_LAST = AXIS_RIGHT_TRIGGER;
  }  // namespace GAMEPAD
}  // namespace ASGE
