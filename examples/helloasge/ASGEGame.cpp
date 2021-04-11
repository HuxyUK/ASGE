#include <Engine/GameSettings.hpp>
#include <Engine/Logger.hpp>
#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>

class ASGENetGame : public ASGE::OGLGame
{
 public:
  explicit ASGENetGame(const ASGE::GameSettings& settings) :
    OGLGame(settings)
  {
    inputs->use_threads = false;
    inputs->addCallbackFnc(ASGE::EventType::E_KEY, &ASGENetGame::keyHandler, this);
    toggleFPS();
    bg = renderer->createUniqueSprite();
    bg->loadTexture("./data/FHD.png");
  }

  ~ASGENetGame() override = default;
  ASGENetGame(const ASGENetGame&) = delete;
  ASGENetGame& operator=(const ASGENetGame&) = delete;

  void keyHandler(ASGE::SharedEventData data)
  {
    const auto* key_event = dynamic_cast<const ASGE::KeyEvent*>(data.get());
    auto key              = key_event->key;
    auto action           = key_event->action;

    if (action == ASGE::KEYS::KEY_PRESSED)
    {
      if (key == ASGE::KEYS::KEY_8)
      {
        renderer->setWindowedMode(ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN);
      }
      if (key == ASGE::KEYS::KEY_9)
      {
        renderer->setWindowedMode(ASGE::GameSettings::WindowMode::WINDOWED);
      }
    }

  }
  void update(const ASGE::GameTime& us) override{};
  void render(const ASGE::GameTime& us) override{renderer->render(*bg);};

 private:
  int key_callback_id = -1; /**< Key Input Callback ID. */
  std::unique_ptr<ASGE::Sprite> bg = nullptr;
};

int main(int /*argc*/, char* /*argv*/[])
{
  ASGE::GameSettings game_settings;
  game_settings.window_title  = "ASGEGame";
  game_settings.window_width  = 1024;
  game_settings.window_height = 768;
  game_settings.mode          = ASGE::GameSettings::WindowMode::WINDOWED;
  game_settings.fixed_ts      = 120;
  game_settings.fps_limit     = 240;
  game_settings.msaa_level    = 1;
  game_settings.vsync         = ASGE::GameSettings::Vsync::DISABLED;

  Logging::INFO("Launching Game!");
  ASGENetGame game(game_settings);
  game.run();
  return 0;
}
