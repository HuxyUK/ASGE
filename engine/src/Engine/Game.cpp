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

#include <Engine/FileIO.hpp>
#include <Engine/Game.hpp>
#include <Engine/Logger.hpp>
#include <PhysFS/PhysFS.hpp>
#include <chrono>
#include <string>
#include <cmath>

void ASGE::Game::toggleFPS() noexcept
{
  show_fps = !show_fps;
}

void ASGE::Game::updateFPS()
{
  static double delta_accumulator = 0;
  static int fps                  = 0;
  static int frames               = 0;

  if (show_fps)
  {
    frames++;
    delta_accumulator += epoch.frame_delta.count();

    std::string fps_str = std::to_string(fps);
    const auto POS_X    = 0.F;
    const auto POS_Y    = 34.F; // renderer->getDefaultFont().line_height;

    auto text = ASGE::Text{ renderer->getFont(0) };
    text.setString(fps_str);
    text.setColour({ 1.0F, 0.2F, 0.5F });
    text.setPosition({ POS_X, POS_Y });
    renderer->renderText(std::move(text));

    if (delta_accumulator >= 1000)
    {
      fps    = static_cast<int>(lround(static_cast<float>(frames) / (delta_accumulator / 1000.0)));
      frames = 0;
      delta_accumulator = 0;
    }
  }
}

void ASGE::Game::initFileIO()
{
  Logging::INFO("=> Initialising File IO");
  PhysFS::init(nullptr);
  PhysFS::setSaneConfig("ASGE", ASGE::SETTINGS.window_title, "dat", false, true);

  auto base_dir = PhysFS::getBaseDir();

  Logging::INFO(base_dir);
  PhysFS::mount(base_dir + "data", "data", true);
  PhysFS::mount(base_dir + "data.pkg", "data", true);
  PhysFS::mount(base_dir + "game.dat", "data", true);

  base_dir = "./";
  PhysFS::mount(base_dir + "data", "data", true);
  PhysFS::mount(base_dir + "data.pkg", "data", true);
  PhysFS::mount(base_dir + "game.dat", "data", true);
  ASGE::FILEIO::printFiles("/data");

  if (!ASGE::SETTINGS.write_dir.empty())
  {
    ASGE::FILEIO::setWriteDir(ASGE::SETTINGS.write_dir, true);
  }

  Logging::INFO("=> File IO initialised");
}

std::chrono::milliseconds ASGE::Game::getGameTime() noexcept
{
  const auto RUNTIME = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::steady_clock::now() - start_time);

  return RUNTIME - paused_time;
}

#include <thread>
int ASGE::Game::run()
{
  renderer->setWindowTitle(ASGE::SETTINGS.window_title.c_str());

  using clock           = std::chrono::steady_clock;
  using ms              = std::chrono::duration<double, std::milli>;
  epoch.fixed_delta     = ms((1 / float(ASGE::SETTINGS.fixed_ts)) * 1000);
  epoch.last_tick_time  = clock::now() - std::chrono::duration_cast<std::chrono::milliseconds>(epoch.fixed_delta);
  epoch.last_frame_time = clock::now() - std::chrono::duration_cast<std::chrono::milliseconds>(
                          ms(1.0 / static_cast<double>(ASGE::SETTINGS.fps_limit)) * 1000);

  while (!exit && !renderer->exit())
  {
    auto tick_start              = clock::now();
    epoch.elapsed                = ASGE::Game::getGameTime();
    constexpr auto MAX_FRAMETIME = std::chrono::milliseconds(200);
    constexpr auto MILLI_IN_SEC  = 1000;

    /*
     * Update Loop
     * The update loop uses a fixed time step. The timing is based on the
     * ASGE::GameSettings and will always use fixed_delta for running.
     * If the updates are lagging, it will attempt to catch-up. To ensure
     * the game doesn't lock-up, it will always render a frame every now
     * and then even if the update is lagging.
     */
    auto accumulator = ms(tick_start - epoch.last_tick_time);
    if(accumulator > epoch.fixed_delta * 2)
    {
      Logging::WARN(
        "Fixed time-step lag is currently " +
        std::to_string(accumulator / epoch.fixed_delta) +
        " updates behind");
    }

    while (accumulator >= epoch.fixed_delta)
    {
      Logging::TRACE("tick start: " + std::to_string(accumulator.count()));
      epoch.last_tick_time = clock::now();
      update(epoch);

      auto tick_delta = clock::now();
      if (ms(tick_delta - epoch.last_frame_time) > MAX_FRAMETIME)
      {
        break; // time to render a frame i.e. slideshow alert
      }

      accumulator -= ms(epoch.fixed_delta - (tick_delta - epoch.last_tick_time));
    }

    // how far along are we to the next "fixed" step
    epoch.distance = accumulator / epoch.fixed_delta;

    /*
     * Render Loop
     * The render loop calls an update function which is basically a variable
     * time-step for simulating render related logic, such as particle systems
     * and for drawing to the GPU.
     */
    epoch.frame_delta = ms(clock::now() - epoch.last_frame_time);
    if (epoch.frame_delta.count() >= (1.0 / static_cast<double>(ASGE::SETTINGS.fps_limit)) * MILLI_IN_SEC)
    {
      epoch.last_frame_time = clock::now();
      beginFrame();
      render(epoch);
      endFrame();
    }
  }

  return EXIT_SUCCESS;
}

void ASGE::Game::signalExit() noexcept
{
  this->exit = true;
}

ASGE::Game::Game(const GameSettings& game_settings)
{
  ASGE::SETTINGS = game_settings;

  // TODO throw exception if FILEIO fails
  initFileIO();
}

ASGE::Game::~Game()
{
  PhysFS::deinit();
}