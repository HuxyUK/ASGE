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

#include "OGLGame.hpp"
#include "OpenGL/GLRenderer.hpp"
#include "OpenGL/GLSprite.hpp"
#include "SplashScreen.hpp"
#include <thread>

bool ASGE::OGLGame::initAPI(const ASGE::GameSettings& settings)
{
  renderer = std::make_unique<GLRenderer>();

  if (!renderer->init(settings))
  {
    return false;
  }

  inputs = renderer->inputPtr();
  if (!inputs)
  {
    return false;
  }

  playSplash();
  renderer->setClearColour(ASGE::COLOURS::CORNFLOWERBLUE);
  return true;
}

void ASGE::OGLGame::playSplash()
{
  #ifndef NO_SPLASH
    //  // play movie here
    //  ASGE::SplashScreen::Animation animation;
    //  animation.duration = 4.5f;
    //  animation.fade_in  = 2.0f;
    //  animation.fade_out = 2.0f;
    //
    //  auto texture = this->renderer->createNonCachedTexture(
    //    SPLASH_IMG_WIDTH,
    //    SPLASH_IMG_HEIGHT,
    //    ASGE::Texture2D::RGB,
    //    (void*)SPLASH_IMG);
    //
    //  auto image = this->renderer->createRawSprite();
    //  if (!image->attach(texture))
    //  {
    //    Logging::ERRORS("Failed to load splash image");
    //  }
    //
    //  image->width(SPLASH_IMG_WIDTH);
    //  image->height(SPLASH_IMG_HEIGHT);
    //
    //  static float FPS = 1.0F / 30.0F;
    //  ASGE::SplashScreen ss{ this->renderer.get(), image, animation };
    //  while (!ss.finished())
    //  {
    //    auto start =
    //    std::chrono::time_point_cast<std::chrono::milliseconds>(
    //            std::chrono::system_clock::now());
    //
    //    this->renderer->setClearColour(ASGE::COLOURS::WHITE);
    //    this->beginFrame();
    //
    //    ss.update(FPS);
    //    ss.render(FPS);
    //    this->endFrame();
    //
    //    auto now =
    //    std::chrono::time_point_cast<std::chrono::milliseconds>(
    //            std::chrono::system_clock::now());
    //
    //    auto delta = now - start;
    //    using namespace std::chrono_literals;
    //    std::this_thread::sleep_for(33.3333ms - delta);
    //  }
    //
    //  delete image;
  #endif
}

bool ASGE::OGLGame::exitAPI() noexcept
{
  return true;
}

void ASGE::OGLGame::beginFrame()
{
  renderer->preRender();
}

void ASGE::OGLGame::endFrame()
{
  if (show_fps)
  {
    dynamic_cast<GLRenderer*>(renderer.get())->renderDebug(updateFPS());
  }

  renderer->postRender();
  renderer->swapBuffers();

  ClearGLErrors(__PRETTY_FUNCTION__);
}

ASGE::OGLGame::OGLGame(const ASGE::GameSettings& settings) : Game(settings)
{
  // TODO throw exception if API fails
  initAPI(settings);
}
