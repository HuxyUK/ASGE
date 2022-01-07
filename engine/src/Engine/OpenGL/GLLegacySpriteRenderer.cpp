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

#include "FileIO.hpp"
#include "GLLegacySpriteRenderer.hpp"
#include "GLRenderer.hpp"
#include "Logger.hpp"
#include "OpenGL/Shaders/GLShaders.fs"
#include "OpenGL/Shaders/GLShaders.vs"

ASGE::GLLegacySpriteRenderer::GLLegacySpriteRenderer()
{
  //find max ubo size...
  Logging::DEBUG(std::to_string(QUAD_STORAGE_SIZE));
  GLint size = 0;
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE , &size);
  Logging::DEBUG("GL_MAX_UNIFORM_BLOCK_SIZE  is " + std::to_string(size) + " bytes.");
}

ASGE::GLLegacySpriteRenderer::~GLLegacySpriteRenderer()
{
  glDeleteBuffers(BUFFER_COUNT, &UBOs[0]);
  glDeleteBuffers(1, &indicies_buffer);

  for (GLsync buffer_sync : syncs)
  {
    glDeleteSync(buffer_sync);
  }
}

bool ASGE::GLLegacySpriteRenderer::init()
{
  SHADER_LIB::GLShader* sprite_shader = initShader(vs_instancing_legacy, fs_instancing);
  basic_sprite_shader                 = sprite_shader->getShaderID();
  basic_text_shader                   = initShader(vs_instancing_legacy, fs_text)->getShaderID();
  active_shader                       = sprite_shader;
  sprite_shader->use();
  setupGlobalShaderData();

  using GLRenderConstants::QUAD_VERTICES;
  UBO_buffer_idx = 0;
  glGenVertexArrays(1, &this->VAO);
  glBindVertexArray(this->VAO);

  // TODO move to actual location layouts inside shader
  int position_loc = sprite_shader->getLocation("position");     // vec2

  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), &QUAD_VERTICES, GL_STATIC_DRAW);
  glVertexAttribPointer(position_loc, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glVertexAttribDivisor(position_loc, 0);
  glEnableVertexAttribArray(position_loc);

  using GLRenderConstants::QUAD_INDICIES;
  glGenBuffers(1, &indicies_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicies_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICIES), &QUAD_INDICIES[0], GL_STATIC_DRAW);

 auto map_uniform_block = [](GLuint shader_id, const std::string& uniform, GLuint binding)
  {
    auto uniform_loc =  glGetUniformBlockIndex(shader_id, uniform.c_str());
    if( uniform_loc != GL_INVALID_INDEX )
    {
      glUniformBlockBinding(shader_id, uniform_loc, binding);
    }
  };

  // TODO move this to a map or something, so I don't have to keep changing all these texts
  //  i.e. PROJECTION_UBO_BIND = global_shader_data...
 map_uniform_block(basic_sprite_shader, "global_shader_data", GLRenderConstants::PROJECTION_UBO_BIND);
 map_uniform_block(basic_text_shader, "global_shader_data", GLRenderConstants::PROJECTION_UBO_BIND);
 map_uniform_block(basic_sprite_shader, "render_quads", GLRenderConstants::QUAD_DATA_UBO_BIND);
 map_uniform_block(basic_text_shader, "render_quads", GLRenderConstants::QUAD_DATA_UBO_BIND);

 constexpr GLbitfield MAPPING_FLAGS = GL_MAP_WRITE_BIT;
 constexpr GLbitfield STORAGE_FLAGS = GL_DYNAMIC_STORAGE_BIT | MAPPING_FLAGS;

 glGenBuffers(BUFFER_COUNT, &UBOs[0]);
  for (const auto& UBO : UBOs)
  {
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, UBOSize(), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  ClearGLErrors(__PRETTY_FUNCTION__);
  return true;
}

int ASGE::GLLegacySpriteRenderer::render(
  std::vector<AnotherRenderBatch>&& batches)
{
  glBindBufferRange(GL_UNIFORM_BUFFER, GLRenderConstants::QUAD_DATA_UBO_BIND, UBOs[UBO_buffer_idx], 0, UBOSize());

  int draw_count = 0;
  for(const auto& batch : batches)
  {
    apply(batch.state);
    bindTexture(batch.texture_id);
    bindShader(batch.shader_id, batch.distance);

    GLint loc = glGetUniformLocation(active_shader->getShaderID(), "quad_buffer_offset");
    glUniform1i(loc, batch.start_idx);
    ClearGLErrors("Setting uniform");

    glDrawElementsInstanced(
      GL_TRIANGLES,
      sizeof(GLRenderConstants::QUAD_INDICIES),
      GL_UNSIGNED_BYTE,
      ((void*)nullptr),
      batch.instance_count); // how may in this batch
    ClearGLErrors("Instance Rendering");

    ++draw_count;
  }

  lockBuffer(syncs[UBO_buffer_idx]);
  UBO_buffer_idx = (UBO_buffer_idx + 1) % BUFFER_COUNT;

  ClearGLErrors(__PRETTY_FUNCTION__);
  return draw_count;
}

constexpr GLsizei ASGE::GLLegacySpriteRenderer::UBOSize() noexcept
{
  return QUAD_STORAGE_SIZE * GLRenderConstants::QUAD_UBO_LIMIT;
}

ASGE::QuadIter
ASGE::GLLegacySpriteRenderer::upload(const ASGE::QuadRange& range)
{
  waitBuffer(syncs[UBO_buffer_idx]);
  GLVMSG(__PRETTY_FUNCTION__, glBindBuffer, GL_UNIFORM_BUFFER, UBOs[UBO_buffer_idx]);

  GLvoid* gpu_mem = GLMSG(
    __PRETTY_FUNCTION__,
    glMapBufferRange,
    GL_UNIFORM_BUFFER,
    0,
    UBOSize(),
    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);

  int64_t uploaded = 0;
  auto cpu_quad = range.begin;

  do
  {
    auto* gpu_quad = &static_cast<GPUQuad*>(gpu_mem)[uploaded++];
    memcpy(gpu_quad, &cpu_quad->gpu_data, sizeof(GPUQuad));

    /// if buffer limit is reached break the loop
    if(uploaded == GLRenderConstants::QUAD_UBO_LIMIT)
    {
      cpu_quad++;
      break;
    }
  } while (cpu_quad++ != range.end);

  /// unmap the buffer
  GLVMSG(__PRETTY_FUNCTION__, glUnmapBuffer,GL_UNIFORM_BUFFER);
  GLVMSG(__PRETTY_FUNCTION__, glBindBuffer,GL_UNIFORM_BUFFER, 0);

  /// return the last quad successfully uploaded
  return std::prev(cpu_quad);
}

ASGE::GLRenderer::RenderLib ASGE::GLLegacySpriteRenderer::getRenderLib() const
{
  return ASGE::GLRenderer::RenderLib::GL_LEGACY;
}
