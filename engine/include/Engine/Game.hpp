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

//! @file Game.hpp
//! @brief Class @ref ASGE::Game

#pragma once
#include <atomic>
#include <memory>

#include "GameSettings.hpp"
#include "GameTime.hpp"
#include "Input.hpp"
#include "Renderer.hpp"

namespace ASGE
{
  /**
   *  @brief The core of the engine.
   *
   *  A skeleton structure and interface for running an instance of a
   *  Game. It stores access to the renderer and the input system.
   *  The input system is intrinsically linked to the renderer
   *  subsystem due to needing access to the window for polling events.
   *  To help speed up development, a typical update and render
   *  function is provided, along with GameTime. The initialisation
   *  code is not created by the Game, but by its derived classes as
   *  the setup of the renderer is platform specific.
   *
   *  @see Input
   *  @see GameTime
   *  @see Renderer
   *
   * <example>
   *  Usage:
   *  @code
   *  #pragma once
   *  #include <Engine/OGLGame.h>
   *  class MyASGEGame : public ASGE::OGLGame
   *  {
   *   public:
   *    explicit MyASGEGame(ASGE::GameSettings settings);
   *    ~MyASGEGame();
   *
   *   private:
   *    void update(const ASGE::GameTime& us) override;
   *    void render() override;
   *  };
   *  @endcode
   * </example>
   */
  class Game
  {
   public:
    /**
     *  Default constructor.
     */
    explicit Game(const GameSettings& game_settings);
    Game(const Game& rhs) = delete;
    Game& operator=(const Game& rhs) = delete;

    /**
     *  Default destructor.
     */
    virtual ~Game();

    /**
     *  @brief Function used to update the game using fixed
     *  time-steps instead of the regular frame update.
     *
     *  This is useful when you only want to progress the
     *  game world i.e. physics with a deterministically
     *  known amount. It is best to use fixed updates as
     *  a divisible or multiple of the FPS count. This allows
     *  a smoother delivery of frame data. For example: 60/120
     *  would deliver one fixed update per two renders.
     *
     *  @param[in] us Game and fixed delta information.
     *  @see GameTime
     */
    virtual void update(const GameTime& us) = 0;

    /**
     * @brief Pure virtual function that sets up the renderer before
     * rendering a frame.
     *
     * Can include flushing of buffers, clearing screens etc.
     * **This is handled inside the platform's specific implementation.**
     */
    virtual void beginFrame() = 0;

    /**
     *  @brief Pure virtual function used to render the game world.
     *  The render pattern is common usage within engines
     *  and will be automatically called every frame. All
     *  game objects to be visualised need to be drawn at this
     *  stage using the Game's renderer.
     *
     *  @param[in] us	Game and Frame delta information.
     *  @see GameTime
     */
    virtual void render(const GameTime& us) = 0;

    /**
     *  @brief Pure virtual function that completes the render frame.
     *
     *  Typically, flushes the render queue, instructs any batched
     *  calls to complete and swaps the video buffers. **This is
     *  handled inside the platform's specific implementation.**
     */
    virtual void endFrame() = 0;

    /**
     *  @brief Pure virtual function for setup and initialisation of API.
     *
     *  **This is handled inside the platform's specific implementation.**
     *
     *  @param[in] settings The window mode to start the game with.
     *  @return The result of the initialisation operation.
     *  @see WindowMode
     */
    virtual bool initAPI() = 0;

    /**
     *  @brief Pure virtual function for terminating the API.
     *
     *  **This is handled inside the platform's specific implementation.**
     *  @return The result of the initialisation operation.
     */
    virtual bool exitAPI() noexcept = 0;

    /**
     *  @brief The main game loop.
     *
     *  Will keep looping until a request to exit is received either
     *  from the renderer or via the signal exit function.
     *
     *  @return The exit code for the game.
     *  @see signalExit
     */
    int run();

    /**
     *  @brief Signals the game should exit.
     *
     *  Will flag the game to stop processing updates and abort the
     *  main game loop, thus exiting the game.
     */
    void signalExit() noexcept;

   protected:
    /**
     *  @brief Toggles the FPS on-screen.
     *
     *  Will cause the game to draw and update the FPS on the screen
     *  every tick along with additional debug information.
     */
    void toggleFPS() noexcept;

    /**
     *  @brief Updates the FPS counter.
     *
     *  Update the FPS calculation every tick. Uses the average number
     *  of frames over a single second.
     */
    void updateFPS();

    /**
     * Initialises the file system.
     */
    void initFileIO();

   protected:
    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
    std::unique_ptr<Renderer> renderer; /**< Unique pointer to the Renderer subsystem. //NOLINT
                                             A smart pointer managing the renderer.
                                             Use renderer.get() to retrieve the raw pointer. */

    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
    std::unique_ptr<Input> inputs; /**< Unique pointer to the Input handler.
                                        A smart pointer managing the input handler.
                                        Use inputs.get() to retrieve the raw pointer. */

    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
    std::atomic<bool> show_fps{ false }; /**< FPS counter. Shows the FPS on screen if set to true. */

    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
    std::atomic<bool> exit{ false }; /**< Exit boolean. If true the game loop will exit. */

   private:
    std::chrono::milliseconds getGameTime() noexcept;
    GameTime epoch; /**< Delta. The frame deltas and total running time. */

    // used to measure the game's running time
    std::chrono::milliseconds paused_time = std::chrono::milliseconds(0);
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
  };
} // namespace ASGE
