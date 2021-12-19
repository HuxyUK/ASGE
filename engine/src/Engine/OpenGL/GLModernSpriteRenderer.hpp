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

#include "CGLSpriteRenderer.hpp"
#include "GLIncludes.hpp"
#include "GLQuad.hpp"
#include "GLRenderBatch.hpp"
#include "GLShader.hpp"
#include "Texture.hpp"
#include <future>
#include <vector>

namespace ASGE
{
	class GLSprite;
	class GLTexture;

	/**
	*  A sprite renderer class designed for OpenGL.
	*  The sprite renderer sets up and manages the render states and drawing
	*  of sprites in the engine. It does this using by processing a render
	*  job. The shader uses instancing which means all data must be aligned
	*  correctly in the shared arrays. To support sort modes, future batch
	*  jobs are supported where the render data is generated but not added
	*  to the renderer. This future job can then be processed at any time
	*  and will be added to the shared arrays. The future jobs will need to 
	*  be ordered before sending them to the sprite renderer. The class also
	*  manages VAO and VBOs. */
	class GLModernSpriteRenderer final : public CGLSpriteRenderer
	{
	public:
   GLModernSpriteRenderer();
		~GLModernSpriteRenderer() override;
    GLModernSpriteRenderer(const GLModernSpriteRenderer&) = delete;
    GLModernSpriteRenderer operator=(const GLModernSpriteRenderer&) = delete;

    bool init() override;
    int render(std::vector<AnotherRenderBatch>&& batches) override;
    QuadIter upload(const QuadRange& range) override;
    [[nodiscard]] GLRenderer::RenderLib getRenderLib() const override;

   private:

    static constexpr GLsizei SSBOSize() noexcept;
    GLuint  SSBO = 0;
		GLuint  SSBO_current_limit = ASGE::GLRenderConstants::MAX_BATCH_COUNT;

		// Triple buffered objects, speeds up AMD/NVIDIA???
    // https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
    unsigned short buffer_idx = 0;
    struct VBOTripleBuffer
    {
      constexpr static int BUFFER_COUNT        = 3;
      std::array<void *, BUFFER_COUNT> buffers = {nullptr};
      std::array<GLsync, BUFFER_COUNT> syncs   = { nullptr};
    };
    VBOTripleBuffer triple_buffer;
  };
}  // namespace ASGE
