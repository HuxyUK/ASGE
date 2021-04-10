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
#pragma once
#include "Engine/RenderTarget.hpp"
#include "GLIncludes.hpp"

namespace ASGE
{
  class GLTexture;

  class GLRenderTarget : public ASGE::RenderTarget
  {
   public:
    GLRenderTarget(ASGE::Renderer* renderer, int width, int height, ASGE::Texture2D::Format format, int count);
    ~GLRenderTarget() override;
    GLRenderTarget(const ASGE::GLRenderTarget&) = delete;
    GLRenderTarget(ASGE::GLRenderTarget&&) = default;
    GLRenderTarget& operator=(const ASGE::GLRenderTarget&) = delete;
    GLRenderTarget& operator=(ASGE::GLRenderTarget&&) = default;

    void use() const;

    [[nodiscard]] const std::vector<ASGE::GLTexture*>& getBuffers() const;
    void setBuffers(const std::vector<ASGE::GLTexture*>& buffers);

   private:
    std::vector<ASGE::GLTexture*> buffers;
    GLuint FBO = 0;
    GLuint DBO = 0;
  };
}  // namespace ASGE