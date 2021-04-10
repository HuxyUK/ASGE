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

void ASGE::Game::toggleFPS() noexcept
{
  show_fps = !show_fps;
}

void ASGE::Game::updateFPS()
{
  static int fps             = 0;
  static auto last_fps_frame = getGameTime();
  frames++;

  if (show_fps)
  {
    std::string fps_str = std::to_string(fps);
    const auto pos_x    = 0;
    const auto pos_y    = 34.F; // renderer->getDefaultFont().line_height;

    auto text = ASGE::Text{ renderer->getFont(0) };
    text.setString("60");
    text.setColour({ 1.0F, 0.2F, 0.5F });
    text.setPosition({ pos_x, pos_y });
    renderer->renderText(std::move(text));

    const auto ELAPSED = getGameTime().count() - last_fps_frame.count();
    if (ELAPSED >= 1000)
    {
      fps            = static_cast<int>(static_cast<float>(frames) / (ELAPSED / 1000.F));
      frames         = 0;
      last_fps_frame = getGameTime();
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
    std::chrono::high_resolution_clock::now() - start_time);

  return RUNTIME - paused_time;
}

int ASGE::Game::run()
{
  renderer->setWindowTitle(ASGE::SETTINGS.window_title.c_str());

  std::chrono::time_point<std::chrono::high_resolution_clock> prev_tick;
  prev_tick         = std::chrono::high_resolution_clock::now();
  using ms          = std::chrono::duration<double, std::milli>;
  epoch.fixed_delta = ms((1 / float(ASGE::SETTINGS.fixed_ts)) * 1000);

  double accumulator = 0.0;
  while (!exit && !renderer->exit())
  {
    auto now   = std::chrono::high_resolution_clock::now();
    auto delta = ms(now - prev_tick);
    prev_tick  = now;

    epoch.elapsed     = ASGE::Game::getGameTime();
    epoch.frame_delta = ms(now - epoch.frame_time);
    epoch.distance    = 0.0;

    constexpr auto MAX_FRAMETIME = std::chrono::milliseconds(200);
    accumulator += delta.count();
    while (accumulator >= epoch.fixed_delta.count())
    {
      fixedUpdate(epoch);
      accumulator -= epoch.fixed_delta.count();

      // just sanity check the last time a frame was rendered
      epoch.frame_delta = ms(std::chrono::high_resolution_clock::now() - epoch.frame_time);
      if (epoch.frame_delta > MAX_FRAMETIME)
      {
        break;
      }
    }

    // how far along are we to the next "fixed" step
    epoch.distance = accumulator / epoch.fixed_delta.count();

    constexpr auto MILLI_IN_SEC = 1000.F;
    if (epoch.frame_delta.count() > (1.0F / static_cast<float>(ASGE::SETTINGS.fps_limit)) * MILLI_IN_SEC)
    {
      epoch.frame_time = std::chrono::high_resolution_clock::now();
      update(epoch);
      beginFrame();
      render();
      endFrame();
    }
  }

  return 0;
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

void ASGE::Game::fixedUpdate(const GameTime& us){};
