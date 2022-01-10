#include <map>
#include <Engine/GameSettings.hpp>
#include <Engine/Logger.hpp>
#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>
#include <Engine/RenderTarget.hpp>

class ASGENetGame : public ASGE::OGLGame
{
 public:
  explicit ASGENetGame(const ASGE::GameSettings& settings) :
    OGLGame(settings)
  {
    renderer->setBaseResolution(1024, 768, ASGE::Resolution::Policy::CENTER);
    inputs->use_threads = false;
    inputs->addCallbackFnc(ASGE::EventType::E_KEY, &ASGENetGame::keyHandler, this);
    bg = renderer->createUniqueSprite();
    bg->loadTexture("/data/images/background.png", static_cast<ASGE::Sprite::AttachMode>(0));
    bg->xPos(-512*0.25F);

    toggleFPS();

    auto texture = renderer->createNonCachedMSAATexture(7000, 2000, ASGE::Texture2D::RGBA);

    robot = renderer->createUniqueSprite();
    robot->loadTexture("/data/images/character_zombie_idle.png", static_cast<ASGE::Sprite::AttachMode>(0));
    robot->xPos((robot->width() * 0.5F));
    robot->yPos(768/2.0F - (robot->height() * 0.5F));

    font_id = renderer->loadFont("/data/fonts/kenvector_future.ttf", 72);

    lh_camera.resize(1024 / 2.0F, 768);
    rh_camera.resize(1024 / 2.0F, 768);
    lh_camera.lookAt({1024 * 0.25F, 768 / 2.0F});
    rh_camera.lookAt({1024 * 0.50F, 768 / 2.0F});
  }

  ~ASGENetGame() override = default;
  ASGENetGame(const ASGENetGame&) = delete;
  ASGENetGame& operator=(const ASGENetGame&) = delete;

 private:


  void keyHandler(ASGE::SharedEventData data)
  {
    const auto* key_event = dynamic_cast<const ASGE::KeyEvent*>(data.get());
    auto key              = key_event->key;
    auto action           = key_event->action;
  }

  void update(const ASGE::GameTime& us) override
  {

  };

  void render(const ASGE::GameTime& us) override
  {
    renderer->setViewport({0,0,3440,1440});
    renderer->setProjectionMatrix({ 0,0,3440,1440 });
    renderer->render(*bg);
//    renderer->setViewport({0,0,512,768});
//    renderer->setProjectionMatrix(lh_camera.getView());
//    renderer->render(*robot);
//    renderer->setViewport({512,0,512,768});
//    renderer->setProjectionMatrix(lh_camera.getView());
//    renderer->render(*robot);
  };

 private:
  int key_callback_id = -1; /**< Key Input Callback ID. */
  std::unique_ptr<ASGE::Sprite> bg = nullptr;
  std::unique_ptr<ASGE::Sprite> zombie = nullptr;
  std::unique_ptr<ASGE::Sprite> robot = nullptr;
  ASGE::Camera lh_camera{};
  ASGE::Camera rh_camera{};
  std::map<int, bool> keys;
  std::map<int, bool> buttons;
  int font_id{0};
  ASGE::RenderTarget* target{ nullptr};
};

int main(int /*argc*/, char* /*argv*/[])
{
  ASGE::GameSettings game_settings;
  game_settings.game_title    = "ASGEGame";
  game_settings.window_width  = 1024;
  game_settings.window_height = 768;
  game_settings.mode          = ASGE::GameSettings::WindowMode::WINDOWED;
  game_settings.fixed_ts      = 60;
  game_settings.fps_limit     = 60;
  game_settings.msaa_level    = 1;
  game_settings.vsync         = ASGE::GameSettings::Vsync::DISABLED;

  Logging::INFO("Launching Game!");
  ASGENetGame game(game_settings);
  game.run();
  return 0;
}
