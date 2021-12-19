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

//! @file RenderTarget.hpp
//! @brief Class @ref ASGE::RenderTarget

#ifndef ASGE_RENDERTARGET_H
#define ASGE_RENDERTARGET_H

#include "Texture.hpp"
#include "Viewport.hpp"
#include <memory>
#include <vector>
namespace ASGE
{
  class Renderer;

  /**
   * @brief RenderTarget is a simple class that describes a render destination.
   *
   * Each render subsystem will be responsible for inheriting from this base
   * class and providing the correct GPU mappings to ensure rendering is
   * redirected correctly.
   */
  class RenderTarget
  {
   public:
    RenderTarget() = default;
    virtual ~RenderTarget() = default;
    RenderTarget(RenderTarget&&) = default;
    RenderTarget& operator=(RenderTarget&&) = default;
    RenderTarget(const RenderTarget&) = default;
    RenderTarget& operator=(const RenderTarget&) = default;
  };
}  // namespace ASGE
#endif // ASGE_RENDERTARGET_H
