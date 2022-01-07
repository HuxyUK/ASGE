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
#include <OpenGL/GLAtlas.hpp>
#include <OpenGL/GLFontSet.hpp>
#include <OpenGL/GLModernSpriteRenderer.hpp>
#include <OpenGL/GLRenderer.hpp>
#include <OpenGL/GLSprite.hpp>
#include <OpenGL/Shaders/GLShaders.fs>
#include <OpenGL/Shaders/GLShaders.vs>
#include <vector>

/**
 *  The constructor for the sprite renderer.
 *  The sprite renderer requires some shared data in order to work. This
 *  data allows it to check which texture and shader is currently loaded.
 *  Storing this info inside the class means it can check to see if it
 *  needs to change the shader or texture before rendering, thus preventing
 *  state changes and expensive load operations.
 *
 *  @param[in] shared_texture_ref The shared reference to the loaded texture.
 *  @param[in] shared_shader_ref The shared reference to the loaded shader.
 *
 *  @see ASGE::GLSpriteBatch
 *  @see ASGE::GLTextRenderer
 */
ASGE::GLModernSpriteRenderer::GLModernSpriteRenderer()
{
  GLint size = 0;
  glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &size);

  std::stringstream ssbo_max_size;
  ssbo_max_size << "GL_MAX_SHADER_STORAGE_BLOCK_SIZE is " << size << " bytes.";
  Logging::DEBUG(ssbo_max_size.str());

  std::stringstream ssbo_allocated;
  ssbo_allocated << "SSBO_ALLOCATED_BUFFER_SIZE is " << SSBOSize() * 3
                 << " bytes.";

  Logging::DEBUG(ssbo_allocated.str());
  Logging::DEBUG("GPUQuad size: " + std::to_string(sizeof(ASGE::GPUQuad)));
  Logging::DEBUG("RenderQuad size: " + std::to_string(sizeof(ASGE::RenderQuad)));
}

/**
 *  The destructor.
 *  Upon destruction the class will signal to the GL subsystem that it
 *  no longer needs the shared buffers for storing vertex data and
 *  instance data such as model matrices. The GPU driver will then free
 *  the memory when it feels happy to do so.
 */
ASGE::GLModernSpriteRenderer::~GLModernSpriteRenderer()
{
  if (glfwGetCurrentContext() != nullptr)
  {
    glDeleteBuffers(1, &SSBO);
    for (GLsync buffer_sync : triple_buffer.syncs)
    {
      glDeleteSync(buffer_sync);
    }
  }
}

/**
 *  Initialises the Sprite Renderer.
 *  Performs initialisation of the renderer including the orthogonal view,
 *  the shader and the shared buffers. Vertex data does not change and is
 *  stored as static drawing data. The other attributes are treated as
 *  dynamic as they can be altered at any time during the program run.
 *
 *  @param[in] ortho_width The width of the orthogonal view.
 *  @param[in] ortho_height The height of the orthogonal view.
 *  @return true if the initialisation is successful.
 */
bool ASGE::GLModernSpriteRenderer::init()
{
  SHADER_LIB::GLShader* sprite_shader = initShader(vs_instancing, fs_instancing);
  basic_sprite_shader                 = sprite_shader->getShaderID();
  basic_text_shader                   = initShader(vs_instancing, fs_text)->getShaderID();
  sprite_shader->use();
  active_shader = sprite_shader;
  setupGlobalShaderData();

  using GLRenderConstants::QUAD_VERTICES;
  buffer_idx = 0;
  glGenVertexArrays(1, &this->VAO);
  glBindVertexArray(this->VAO);

  // TODO move to actual location layouts inside shader
  int position_loc = sprite_shader->getLocation("position"); // vec2

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW);
  glVertexAttribPointer(position_loc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glVertexAttribDivisor(position_loc, 0);
  glEnableVertexAttribArray(position_loc);

  constexpr GLbitfield MAPPING_FLAGS = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
  constexpr GLbitfield STORAGE_FLAGS = GL_DYNAMIC_STORAGE_BIT | MAPPING_FLAGS;

  const auto MAX_SSBO_CAPACITY = SSBOSize() * ASGE::GLModernSpriteRenderer::VBOTripleBuffer::BUFFER_COUNT;

  glCreateBuffers(1, &SSBO);
  glBindBufferRange(GL_SHADER_STORAGE_BUFFER, GLRenderConstants::QUAD_DATA_SSBO_BIND, SSBO, 0, MAX_SSBO_CAPACITY);
  glNamedBufferStorage(SSBO, MAX_SSBO_CAPACITY, nullptr, STORAGE_FLAGS);

  triple_buffer.buffers[0] = glMapNamedBufferRange(SSBO, 0, MAX_SSBO_CAPACITY, MAPPING_FLAGS | GL_MAP_FLUSH_EXPLICIT_BIT);
  triple_buffer.buffers[1] = static_cast<GLbyte*>(triple_buffer.buffers[0]) + SSBOSize();
  triple_buffer.buffers[2] = static_cast<GLbyte*>(triple_buffer.buffers[1]) + SSBOSize();

  checkForErrors();
  return true;
}

/**
 *  Generates matrix data for the sprite.
 *  The shader utilises sprites to scale, position and translate the
 *  the sprites on the screen. This function will take a render job
 *  and generate a matrix that satisifies the job's requirements. Rotation
 *  occurs around the mid-point, but other operations use the origin 0,0.
 *
 *  @param[in] job The render job to use.
 *  @param[out] model_matrix The resultant matrix.
 */

/**
 *  Generates the vector data for rgba colour.
 *  Using the data inside the render job, the rgb and the alpha component are
 *  combined into a single glm::vec4. This can now then be passed onto the
 * shader for rendering.
 *
 *  param [in] job The current sprite being rendered
 *  param [out] rgba The vec4 representation of colour
 */

constexpr GLsizei ASGE::GLModernSpriteRenderer::SSBOSize() noexcept
{
  return GLRenderConstants::MATRIX_STORAGE_SIZE +
         GLRenderConstants::UV_STORAGE_SIZE +
         GLRenderConstants::COLOUR_STORAGE_SIZE;
}

int ASGE::GLModernSpriteRenderer::render(
  std::vector<AnotherRenderBatch>&& batches)
{
  int draw_count = 0;
  for (const auto& batch : batches)
  {
    apply(batch.state);
    bindTexture(batch.texture_id);
    bindShader(batch.shader_id, batch.distance);

    glUniform1i(GLRenderConstants::OFFSET_UBO_BIND, batch.start_idx);
    ClearGLErrors("Setting uniform");

    glDrawElementsInstancedBaseInstance(
      GL_TRIANGLES,
      sizeof(GLRenderConstants::QUAD_INDICIES),
      GL_UNSIGNED_BYTE,
      GLRenderConstants::QUAD_INDICIES,
      batch.instance_count,
      0);

    ++draw_count;
  }

  lockBuffer(triple_buffer.syncs[buffer_idx]);
  buffer_idx = (buffer_idx + 1) % VBOTripleBuffer::BUFFER_COUNT;
  return draw_count;
}

ASGE::QuadIter
ASGE::GLModernSpriteRenderer::upload(const ASGE::QuadRange& range)
{
  waitBuffer(triple_buffer.syncs[buffer_idx]);
  auto *gpu_data = triple_buffer.buffers[buffer_idx];

  GLsizeiptr uploaded = 0;
  auto cpu_quad = range.begin;
  do
  {
    auto *gpu_quad = &reinterpret_cast<GPUQuad*>(gpu_data)[uploaded++];
    memcpy(gpu_quad, &cpu_quad->gpu_data, sizeof(GPUQuad));
    if (uploaded == SSBO_current_limit)
    {
      Logging::DEBUG("Reached SSBO Limit");
      break;
    }
  } while (cpu_quad++ != range.end);

  /// flush the memory to the GPU
  GLVMSG(
    __PRETTY_FUNCTION__,
    glFlushMappedNamedBufferRange,
    SSBO,
    SSBOSize() * buffer_idx,
    QUAD_STORAGE_SIZE * uploaded);

  /// rebind the range used in the SSBO bind
  glBindBufferRange(
    GL_SHADER_STORAGE_BUFFER,
    GLRenderConstants::QUAD_DATA_SSBO_BIND,
    SSBO,
    SSBOSize() * buffer_idx,
    QUAD_STORAGE_SIZE * uploaded);

  return std::prev(range.begin + uploaded);
}

ASGE::GLRenderer::RenderLib ASGE::GLModernSpriteRenderer::getRenderLib() const
{
  return ASGE::GLRenderer::RenderLib::GL_MODERN;
}
