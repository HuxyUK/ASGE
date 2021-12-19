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

#include "Logger.hpp"
#include "Renderer.hpp"
#include "SplashScreen.hpp"

void ASGE::SplashScreen::update(float delta_time)
{
  if (!animation.isFinished())
  {
    animation.elapsed += delta_time;
  }
}

ASGE::SplashScreen::SplashScreen(
  ASGE::Renderer* renderer_in, const std::string& path, Animation&& anim_in) :
  renderer(renderer_in),
  image(renderer->createRawSprite()), animation(anim_in)
{

  if (!image->loadTexture(path))
  {
    Logging::ERRORS("Failed to load splash image");
  }

  auto height = renderer->windowHeight();
  auto width  = renderer->windowWidth();

  image->yPos((height - image->height()) / 2);
  image->xPos((width - image->width()) / 2);
}

void ASGE::SplashScreen::reset() noexcept
{
  this->animation.elapsed = 0.0F;
}

void ASGE::SplashScreen::render(float /*delta_time*/)
{
  auto opacity = 1.0F;
  if (animation.elapsed < animation.fade_in)
  {
    opacity *= animation.elapsed / animation.fade_in;
  }
  else if ((animation.fade_out != 0.0F) && animation.elapsed > animation.duration - animation.fade_out)
  {
    opacity *= (animation.duration - animation.elapsed) / animation.fade_out;
  }

  this->image->opacity(opacity);
  renderer->render(*this->image);
}

bool ASGE::SplashScreen::finished() const noexcept
{
  return animation.isFinished();
}

ASGE::SplashScreen::SplashScreen(ASGE::Renderer* renderer, ASGE::Sprite* data, Animation& anim) :
  renderer(renderer), image(data), animation(anim)
{
  auto height = renderer->windowHeight();
  auto width  = renderer->windowWidth();

  image->yPos((height - image->height()) / 2);
  image->xPos((width - image->width()) / 2);
}
