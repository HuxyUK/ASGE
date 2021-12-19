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
#include "RenderTarget.hpp"
#include "GLIncludes.hpp"

namespace ASGE
{
  class GLTexture;

  /**
   * An MSAA OpenGL implementation of an ASGE::RenderTarget
   *
   * Creates and manages render targets. Each render targets may have 1 or more number
   * of textures attached to it. By default, multi-sampling is enabled meaning before
   * the textures can be sampled they will need to be resolved.
   *
   * The class also manually maintains a list of all the textures attached to frame
   * buffer. When the render target is destroyed the memory will be automatically
   * freed. So, care should be taken to ensure any references or pointers held to the
   * textures are also destroyed.
   *
   * @see ASGE::Font
   */
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
    ASGE::Texture2D* resolve(unsigned int index);
    const std::vector<std::unique_ptr<ASGE::GLTexture>>& resolve();
    [[nodiscard]] const std::vector<std::unique_ptr<ASGE::GLTexture>>& getResolved() const;

   private:
    void createFboWithMultiSampledAttachments(
      Renderer* renderer, int width, int height, Texture2D::Format format, int count);
    void createFboWithAttachments(
      Renderer* renderer, int width, int height, Texture2D::Format format, int count);

    GLuint DBO = 0; //!< The resolved depth buffer attachment
    GLuint FBO = 0; //!< The resolved frame buffer object
    GLuint MSAA_DBO = 0; //!< The MSAA depth buffer attachment
    GLuint MSAA_FBO = 0; //!< The MSAA frame buffer object

    //! MSAA textures used by the frame buffer.
    std::vector<std::unique_ptr<ASGE::GLTexture>> msaa_textures;

    //! Resolved textures used to store the downscaled textures.
    std::vector<std::unique_ptr<ASGE::GLTexture>> resolved_textures;
  };
}  // namespace ASGE
