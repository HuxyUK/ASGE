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

#ifndef ASGE_GLRENDERERBASE_HPP
#define ASGE_GLRENDERERBASE_HPP

#include "GLIncludes.hpp"
#include "GLRenderBatch.hpp"
#include "GLRenderer.hpp"
#include "GLShader.hpp"
#include <vector>

namespace ASGE
{

  /**
   * The platform specific implementation of an ASGE renderer based
   * on the use of the OpenGL framework. Requires an additional
   * concrete implementation due to the varying versions of OpenGL
   * support.
   */
  class CGLSpriteRenderer
  {

   public:
    CGLSpriteRenderer() = default;
    virtual ~CGLSpriteRenderer();
    virtual bool init() = 0;
    virtual QuadIter upload(const QuadRange& range) = 0;
    virtual int render(std::vector<AnotherRenderBatch>&& batches) = 0;

    CGLSpriteRenderer(const CGLSpriteRenderer&) = delete;
    CGLSpriteRenderer& operator=(const CGLSpriteRenderer&) = delete;

    ASGE::SHADER_LIB::GLShader* initShader(const std::string& vertex_shader, const std::string& fragment_shader);
    void quadGen(const GLSprite& sprite, GPUQuad& dest) noexcept;
    void createCharQuad( const GLCharRender& character, const ASGE::Colour& colour, ASGE::GPUQuad& quad) const;
    void clearActiveRenderState();

    [[nodiscard]] virtual GLRenderer::RenderLib getRenderLib() const = 0;
    [[nodiscard]] unsigned int getDefaultTextShaderID() const noexcept;
    [[nodiscard]] unsigned int getBasicSpriteShaderID() const noexcept;

    void setActiveShader(ASGE::SHADER_LIB::GLShader* shader);
    ASGE::SHADER_LIB::GLShader* activeShader();

   protected:
    GLuint  basic_sprite_shader = 0;
    GLuint  basic_text_shader = 0;
    GLuint  vertex_buffer = 0;
    GLuint  VAO = 0;
    GLuint  current_loaded_texture = 0;
    GLuint  current_loaded_shader = 0;
    GLuint  shader_data_location = 0;
    RenderState* active_render_state {nullptr};
    SHADER_LIB::GLShader* active_shader = nullptr;

    void generateSpriteMatrixData(const ASGE::GLSprite& sprite, glm::mat4* model_matrix) const;
    void generateColourData(const ASGE::GLSprite& sprite, glm::vec4* rgba) const;
    void generateUvData(const ASGE::GLSprite& sprite, GLfloat[GLRenderConstants::UVS_PER_QUAD]) const;
    void checkForErrors() const;
    bool bindShader(GLuint shader_id, GLfloat distance) noexcept;
    void lockBuffer(GLsync& sync_prim);
    void waitBuffer(GLsync& sync_prim);
    bool bindTexture(GLuint texture_id);

    // work in progress
    void apply(ASGE::RenderState* state);
    void setupGlobalShaderData();
  };

}
#endif // ASGE_GLRENDERERBASE_HPP
