#include <Engine/GameSettings.hpp>
#include <Engine/Logger.hpp>
#include <Engine/OGLGame.hpp>

class ASGENetGame : public ASGE::OGLGame
{
 public:
  explicit ASGENetGame(const ASGE::GameSettings& settings) :
    OGLGame(settings)
  {
  }

  ~ASGENetGame() override = default;
  ASGENetGame(const ASGENetGame&) = delete;
  ASGENetGame& operator=(const ASGENetGame&) = delete;

  void keyHandler(ASGE::SharedEventData data){};
  void update(const ASGE::GameTime& us) override{};
  void render() override{};
  void fixedUpdate(const ASGE::GameTime& us) override{};

 private:
  int key_callback_id = -1; /**< Key Input Callback ID. */
};

int main(int /*argc*/, char* /*argv*/[])
{
  ASGE::GameSettings game_settings;
  game_settings.window_title  = "ASGEGame";
  game_settings.window_width  = 1024;
  game_settings.window_height = 768;
  game_settings.mode          = ASGE::GameSettings::WindowMode::WINDOWED;
  game_settings.fixed_ts      = 30;
  game_settings.fps_limit     = 120;
  game_settings.msaa_level    = 1;

  Logging::INFO("Launching Game!");
  ASGENetGame game(game_settings);
  game.run();
  return 0;
}
