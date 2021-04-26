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

//! @file Keys.hpp
//! @brief Namespace @ref ASGE::KEYS

#pragma once
/**
 * @namespace ASGE::KEYS
 * @brief input key bindings
 * @details http://www.glfw.org/docs/latest/group__keys.html
 */
#include <bitset>
#include <magic_enum.hpp>
namespace ASGE::KEYS
{
  constexpr int KEY_RELEASED = 0; /**< Key Release. The key or mouse button was pressed. */
  constexpr int KEY_PRESSED  = 1; /**< Key Press. The key or mouse button was released. */
  constexpr int KEY_REPEATED = 2; /**< Key Repeat. The key was held down until it repeated. */

  constexpr int KEY_SPACE         = 32;
  constexpr int KEY_APOSTROPHE    = 39;
  constexpr int KEY_COMMA         = 44;
  constexpr int KEY_MINUS         = 45;
  constexpr int KEY_PERIOD        = 46;
  constexpr int KEY_SLASH         = 47;
  constexpr int KEY_0             = 48;
  constexpr int KEY_1             = 49;
  constexpr int KEY_2             = 50;
  constexpr int KEY_3             = 51;
  constexpr int KEY_4             = 52;
  constexpr int KEY_5             = 53;
  constexpr int KEY_6             = 54;
  constexpr int KEY_7             = 55;
  constexpr int KEY_8             = 56;
  constexpr int KEY_9             = 57;
  constexpr int KEY_SEMICOLON     = 59;
  constexpr int KEY_EQUAL         = 61;
  constexpr int KEY_A             = 65;
  constexpr int KEY_B             = 66;
  constexpr int KEY_C             = 67;
  constexpr int KEY_D             = 68;
  constexpr int KEY_E             = 69;
  constexpr int KEY_F             = 70;
  constexpr int KEY_G             = 71;
  constexpr int KEY_H             = 72;
  constexpr int KEY_I             = 73;
  constexpr int KEY_J             = 74;
  constexpr int KEY_K             = 75;
  constexpr int KEY_L             = 76;
  constexpr int KEY_M             = 77;
  constexpr int KEY_N             = 78;
  constexpr int KEY_O             = 79;
  constexpr int KEY_P             = 80;
  constexpr int KEY_Q             = 81;
  constexpr int KEY_R             = 82;
  constexpr int KEY_S             = 83;
  constexpr int KEY_T             = 84;
  constexpr int KEY_U             = 85;
  constexpr int KEY_V             = 86;
  constexpr int KEY_W             = 87;
  constexpr int KEY_X             = 88;
  constexpr int KEY_Y             = 89;
  constexpr int KEY_Z             = 90;
  constexpr int KEY_LEFT_BRACKET  = 91;
  constexpr int KEY_BACKSLASH     = 92;
  constexpr int KEY_RIGHT_BRACKET = 93;
  constexpr int KEY_GRAVE_ACCENT  = 96;
  constexpr int KEY_WORLD_1       = 161;
  constexpr int KEY_WORLD_2       = 162;
  constexpr int KEY_ESCAPE        = 256;
  constexpr int KEY_ENTER         = 257;
  constexpr int KEY_TAB           = 258;
  constexpr int KEY_BACKSPACE     = 259;
  constexpr int KEY_DELETE        = 261;
  constexpr int KEY_RIGHT         = 262;
  constexpr int KEY_LEFT          = 263;
  constexpr int KEY_DOWN          = 264;
  constexpr int KEY_UP            = 265;
  constexpr int KEY_MAX           = KEY_UP;

  enum ModKeys
  {
    MOD_KEY_SHIFT,
    MOD_KEY_CONTROL,
    MOD_KEY_ALT,
    MOD_KEY_SUPER,
    MOD_KEY_CAPS_LOCK,
    MOD_KEY_NUM_LOCK
  };

  // using namespace magic_enum::bitwise_operators;
  using MODS = std::bitset<magic_enum::enum_count<ModKeys>()>;
} // namespace ASGE::KEYS
