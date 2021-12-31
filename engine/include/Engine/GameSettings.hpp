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

//! @file GameSettings.hpp
//! @brief
//! Struct @ref ASGE::GameSettings,
//! Enum @ref ASGE::GameSettings::WindowMode,
//! Enum @ref ASGE::GameSettings::MagFilter,
//! Enum @ref ASGE::GameSettings::Vsync

#ifndef ASGE_GAMESETTINGS_HPP
#define ASGE_GAMESETTINGS_HPP
#include <string>
namespace ASGE
{
  /**
   * @brief GameSettings allows you to configure the game window and
   * its initial state upon construction of the game. You could easily
   * use a GUI to create an instance of GameSettings and then construct
   * the game using it.
   *
   * @code
   *   ASGE::GameSettings game_settings;
   *   game_settings.window_width = 1920;
   *   game_settings.window_height = 1080;
   *   game_settings.window_bpp = 32;
   *   game_settings.mode = ASGE::GameSettings::WindowMode::BORDERLESS_WINDOWED;
   *   game_settings.msaa_level = 1;
   *   game_settings.mag_filter = ASGE::GameSettings::MagFilter::NEAREST;
   *   game_settings.fps_limit = 360;
   *   game_settings.fixed_ts = 240;
   *   game_settings.vsync = ASGE::GameSettings::Vsync::DISABLED;
   *   SampleGame game(game_settings);
   *@endcode
   */
  struct GameSettings
  {
    enum class WindowMode
    {
      EXCLUSIVE_FULLSCREEN  = 0, /**< Fullscreen without decorations. */
      WINDOWED = 1,              /**< Windowed desktop mode. */
      BORDERLESS_WINDOWED = 2,   /**< Borderless window desktop mode. */
      BORDERLESS_FULLSCREEN = 3, /**< Borderless full screen window mode. */
    };

    enum class MagFilter : uint32_t
    {
      NEAREST, /**< Takes the nearest pixel in manhattan distance.*/
      LINEAR   /**< Takes the weighted average of all 4 pixels.*/
    };

    enum class Vsync
    {
      ENABLED,  /**< Forces vsync at all times. FPS will be controlled by monitors refresh.*/
      ADAPTIVE, /**< Enables vsync when able to meet monitor's refresh, disables when below.*/
      DISABLED  /**< Allow the game to run unrestricted.*/
    };

    GameSettings()  = default;
    ~GameSettings() = default;

    int window_width{ 800 }; /**< The width of the GL Window to create. */                   // NOLINT
    int window_height{ 640 }; /**< The height of the GL Window to create. */                 // NOLINT
    int window_bpp{ 32 }; /**< The Bits Per Pixel (BPP) of the window. */                    // NOLINT
    int msaa_level{ 32 }; /**< MSAA can help with edges, but be lowered if FPS is poor. */   // NOLINT
    int fps_limit{ 60 }; /**< The engine will attempt to never exceed this tick rate. */     // NOLINT
    int fixed_ts{ fps_limit * 2 }; /**< The delta between fixed time-steps. */               // NOLINT
    int anisotropic{ 16 }; /**< Improves filtering at oblique angles. Not useful for 2D. */  // NOLINT

    std::string write_dir{}; /**< The default write directory for ASGE IO. */
    std::string game_title{ "My ASGE Game" }; /**< The window title. */
    WindowMode mode{ WindowMode::WINDOWED }; /**< The window mode i.e. Fullscreen. */
    MagFilter mag_filter {MagFilter::LINEAR}; /**< Textures will use this filter by default. */
    Vsync vsync {Vsync::ADAPTIVE};
  };

}  // namespace ASGE
#endif // ASGE_GAMESETTINGS_HPP

