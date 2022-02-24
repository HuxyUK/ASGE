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

#include "FileIO.hpp"
#include "Game.hpp"
#include "Logger.hpp"
#include <PhysFS/PhysFS.hpp>
#include <chrono>
#include <string>
#include <cmath>

void ASGE::Game::toggleFPS() noexcept
{
  show_fps.store(!show_fps.load());
}

int ASGE::Game::updateFPS()
{
  static double delta_accumulator = 0;
  static int fps                  = 60;
  static int frames               = 0;

  if (show_fps)
  {
    frames++;
    delta_accumulator += epoch.frame_delta.count();

    if (delta_accumulator >= 1000)
    {
      fps    = static_cast<int>(lround(static_cast<float>(frames) / (delta_accumulator / 1000.0)));
      frames = 0;
      delta_accumulator = 0;
    }
  }

  return fps;
}

void ASGE::Game::initFileIO(const ASGE::GameSettings& settings)
{
  Logging::INFO("=> Initialising File IO");
  PhysFS::init(nullptr);
  PhysFS::setSaneConfig("ASGE", settings.game_title, "dat", false, true);

  auto base_dir = PhysFS::getBaseDir();

  Logging::INFO("=> Working Directory: " + base_dir);
  PhysFS::mount(base_dir + "data", "data", true);
  PhysFS::mount(base_dir + "data.pkg", "data", true);
  PhysFS::mount(base_dir + "game.dat", "data", true);

  base_dir = "./";
  PhysFS::mount(base_dir + "data", "data", true);
  PhysFS::mount(base_dir + "data.pkg", "data", true);
  PhysFS::mount(base_dir + "game.dat", "data", true);
  ASGE::FILEIO::printFiles("/data");

  if (!settings.write_dir.empty())
  {
    ASGE::FILEIO::setWriteDir(settings.write_dir, true);
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
  renderer->setWindowTitle(title().c_str());

  using clock               = std::chrono::steady_clock;
  using ms                  = std::chrono::duration<double, std::milli>;
  epoch.fixed_delta         = ms((1 / float(fixedTimeStep())) * 1000);
  epoch.last_fixedstep_time = clock::now() - std::chrono::duration_cast<std::chrono::milliseconds>(epoch.fixed_delta);
  epoch.last_frame_time     = clock::now() - std::chrono::duration_cast<std::chrono::milliseconds>(
                              ms(1.0 / static_cast<double>(fpsLimit())) * 1000);

  while (!exit && !renderer->exit())
  {
    auto tick_start              = clock::now();
    epoch.elapsed                = ASGE::Game::getGameTime();
    constexpr auto MAX_FRAMETIME = std::chrono::milliseconds(200);
    constexpr auto MILLI_IN_SEC  = 1000;

    /*
     * Fixed-Update Loop
     * The fixed update loop uses a fixed time step. The timing is based on
     * the ASGE::GameSettings and will always use fixed_delta for running.
     * If the updates are lagging, it will attempt to catch-up. To ensure
     * the game doesn't lock up, it will always render a frame every now
     * and then even if the update is lagging.
     */
    auto accumulator = ms(tick_start - epoch.last_fixedstep_time);
    if(accumulator > epoch.fixed_delta * 5)
    {
      Logging::WARN(
        "Fixed time-step lag is currently " +
        std::to_string(accumulator / epoch.fixed_delta) +
        " updates behind");
    }

    bool timed_out = false;
    while (accumulator >= epoch.fixed_delta)
    {
      Logging::TRACE("tick start: " + std::to_string(accumulator.count()));
      epoch.last_fixedstep_time = clock::now();
      fixedUpdate(epoch);

      auto current_tick_time = clock::now();
      if (ms(current_tick_time - epoch.last_frame_time) > MAX_FRAMETIME)
      {
        timed_out = true;
        break; // time to render a frame i.e. slideshow alert
      }

      accumulator -= ms(epoch.fixed_delta - (current_tick_time - epoch.last_fixedstep_time));
    }

    // how far along are we to the next "fixed" step
    epoch.distance = accumulator / epoch.fixed_delta;


    /*
     * Render Loop
     * The render loop calls an update function which is basically a variable
     * time-step for simulating render related logic, such as particle systems
     * and for drawing to the GPU.
     */

    epoch.frame_delta = timed_out ? MAX_FRAMETIME: ms(clock::now() - epoch.last_frame_time);
    if (timed_out || epoch.frame_delta.count() >= (1.0 / static_cast<double>(fpsLimit())) * MILLI_IN_SEC)
    {
      epoch.last_frame_time = clock::now();
      update(epoch);
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

ASGE::Game::Game(const GameSettings& game_settings) :
  fixed_ts(game_settings.fixed_ts),
  fps_limit(game_settings.fps_limit),
  game_title(game_settings.game_title)
{
  // TODO throw exception if FILEIO fails
  initFileIO(game_settings);
}

ASGE::Game::~Game()
{
  PhysFS::deinit();
}

void ASGE::Game::fixedUpdate(const ASGE::GameTime &/*us*/)
{

}

const std::string& ASGE::Game::title() const
{
  return game_title;
}

uint32_t ASGE::Game::fpsLimit() const
{
  return fps_limit;
}

uint32_t ASGE::Game::fixedTimeStep() const
{
  return fixed_ts;
}
