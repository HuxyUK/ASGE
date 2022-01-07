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

#ifndef ASGE_GLLEGACYSPRITERENDERER_HPP
#define ASGE_GLLEGACYSPRITERENDERER_HPP
#include "CGLSpriteRenderer.hpp"
namespace ASGE
{
  /**
   * The GLLegacySpriteRenderer is designed to remain complaint with
   * OpenGL 3.3. Many of the newer rendering functions will not be
   * used, including the use of SSBOs and named bindings. Mainly,
   * this class is motivated around the use of OSX but also older GPUs
   * such as intel iGPU's.
   */
  class GLLegacySpriteRenderer : public CGLSpriteRenderer
  {
   public:
    GLLegacySpriteRenderer();
    ~GLLegacySpriteRenderer() override;
    GLLegacySpriteRenderer(const GLLegacySpriteRenderer&) = delete;
    GLLegacySpriteRenderer operator=(const GLLegacySpriteRenderer&) = delete;

    bool init() override;
    QuadIter upload(const QuadRange& range) override;
    int render(std::vector<AnotherRenderBatch>&& batches) override;
    [[nodiscard]] GLRenderer::RenderLib getRenderLib() const override;

   private:
    static constexpr GLsizei UBOSize() noexcept;
    static constexpr GLsizei BUFFER_COUNT = 3;
    std::array<GLuint, BUFFER_COUNT> UBOs  {0};
    std::array<GLsync, BUFFER_COUNT> syncs {nullptr};
    GLuint UBO_buffer_idx = 0;
    GLuint indicies_buffer = 0;
  };
}  // namespace ASGE
#endif // ASGE_GLLEGACYSPRITERENDERER_HPP
