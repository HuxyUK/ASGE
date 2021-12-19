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

#ifndef ASGE_MOVIE_H
#define ASGE_MOVIE_H

#include "Sprite.hpp"
namespace ASGE
{
  class SplashScreen
  {
   public:
    struct Animation
    {
      float duration = 0.F;
      float elapsed  = 0.F;
      float fade_in  = 0.F;
      float fade_out = 0.F;
      [[nodiscard]] bool isFinished() const noexcept { return elapsed >= duration; }
    };

    ASGE::Renderer* renderer;

   private:
    ASGE::Sprite* image;
    Animation animation;

   public:
    SplashScreen(ASGE::Renderer*, const std::string&, Animation&&);
    SplashScreen(ASGE::Renderer*, ASGE::Sprite*, Animation&);
    ~SplashScreen() = default;

    void reset() noexcept;
    void update(float delta_time = 0.0F);
    void render(float delta_time = 0.0F);
    bool finished() const noexcept;
  };
} // namespace ASGE

#endif // ASGE_MOVIE_H
