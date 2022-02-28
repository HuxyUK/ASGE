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

#include "CGLSpriteRenderer.hpp"
#include "GLRenderer.hpp"
#include "Logger.hpp"
#include "OpenGL/GLAtlas.hpp"
#include "OpenGL/GLFontSet.hpp"
#include "OpenGL/GLSprite.hpp"

namespace ASGE
{
  namespace
  {
    struct SHADER_DATA
    {
      glm::mat4 projection;
    };
  }
}

void ASGE::CGLSpriteRenderer::checkForErrors() const
{
  while (auto error = glGetError())
  {
    Logging::ERRORS(std::to_string(error));
  }
}

ASGE::SHADER_LIB::GLShader* ASGE::CGLSpriteRenderer::initShader(
  const std::string& vertex_shader, const std::string& fragment_shader)
{
  SHADER_LIB::GLShader shader;
  if (shader.compile(vertex_shader.c_str(), fragment_shader.c_str()))
  // if(shader.loadShaderFromFile("vertex.glsl", fragment_shader.c_str()))
  {
    GLRenderer::shaders.emplace_back(std::move(shader));
    return &GLRenderer::shaders.back();
  }

  return nullptr;
}

bool ASGE::CGLSpriteRenderer::bindShader(GLuint shader_id, GLfloat distance) noexcept
{
  shader_id == 0 ? shader_id = getBasicSpriteShaderID() : shader_id;

  if(active_shader == nullptr || current_loaded_shader != shader_id)
  {
    auto iter = std::find(GLRenderer::shaders.begin(), GLRenderer::shaders.end(), shader_id);
    if (iter == GLRenderer::shaders.end())
    {
      return false;
    }

    active_shader = &(*iter);
    active_shader->use();
    current_loaded_shader = shader_id;
  }

  //TODO: move towards a more agnostic way of applying uniforms
  if(distance)
  {
    active_shader->getUniform("distance_factor")->set(distance);
  }

  return true;
}

void ASGE::CGLSpriteRenderer::lockBuffer(GLsync& sync_prim)
{
  if (sync_prim != nullptr)
  {
    glDeleteSync(sync_prim);
  }

  sync_prim = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void ASGE::CGLSpriteRenderer::waitBuffer(GLsync& sync_prim)
{
  if (sync_prim != nullptr)
  {
    while (true)
    {
      GLenum wait_sync = glClientWaitSync(sync_prim, GL_SYNC_FLUSH_COMMANDS_BIT, 1);

      if (wait_sync == GL_ALREADY_SIGNALED || wait_sync == GL_CONDITION_SATISFIED)
      {
        return;
      }
    }
  }
}

bool ASGE::CGLSpriteRenderer::bindTexture(GLuint texture_id)
{
  if (current_loaded_texture != texture_id)
  {
    glActiveTexture(GL_TEXTURE0);
    GLVMSG("Binding Texture", glBindTexture, GL_TEXTURE_2D, texture_id);
    current_loaded_texture = texture_id;
    return true;
  }
  return false;
}

unsigned int ASGE::CGLSpriteRenderer::getDefaultTextShaderID() const noexcept
{
  return basic_text_shader;
}

ASGE::CGLSpriteRenderer::~CGLSpriteRenderer()
{
  if (glfwGetCurrentContext() != nullptr)
  {
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &shader_data_location);
  }
}

void ASGE::CGLSpriteRenderer::generateSpriteMatrixData(
  const ASGE::GLSprite& sprite, glm::mat4* model_matrix_ptr) const
{
  // position * rotation * scale
  // translation to position, followed by rotation
  // around the middle of the sprite, finished with
  // a scale and polish

  glm::mat4& model_matrix = *model_matrix_ptr;
  model_matrix            = { glm::mat4(1.F) };

  model_matrix = glm::translate(
    model_matrix,
    glm::vec3(sprite.xPos(), sprite.yPos(), sprite.getGlobalZOrder()));

  model_matrix = glm::translate(
    model_matrix,
    glm::vec3(
      0.5F * sprite.width() * sprite.scale(),
      0.5F * sprite.height() * sprite.scale(),
      0.0F));

  model_matrix = glm::rotate(model_matrix, sprite.rotationInRadians(), glm::vec3{ 0, 0, 1 });

  model_matrix = glm::translate(
    model_matrix,
    glm::vec3(
      -0.5F* sprite.width() * sprite.scale(),
      -0.5F * sprite.height() * sprite.scale(),
      0.0F));

  model_matrix = glm::scale(
    model_matrix,
    glm::vec3(
      sprite.width() * sprite.scale(), sprite.height() * sprite.scale(), 1.0F));
}

void ASGE::CGLSpriteRenderer::generateColourData(const ASGE::GLSprite& sprite, glm::vec4* rgba) const
{
  *rgba = glm::vec4{ sprite.colour().r, sprite.colour().g, sprite.colour().b, sprite.opacity() };
}

void ASGE::CGLSpriteRenderer::generateUvData(
  const ASGE::GLSprite& sprite, GLfloat uvs[GLRenderConstants::UVS_PER_QUAD]) const
{
  auto src_rect = glm::vec4(
    sprite.srcRect()[0],
    sprite.srcRect()[1],
    sprite.srcRect()[2],
    sprite.srcRect()[3]);

  for (int i = 0; i < GLRenderConstants::UVS_PER_QUAD; i += 4)
  {
    auto uv_x = uvs[i];
    uv_x *= src_rect[2] / sprite.getTexture()->getWidth();
    uv_x += src_rect[0] / sprite.getTexture()->getWidth();

    uvs[i] = uv_x;
  }

  for (int i = 1; i < GLRenderConstants::UVS_PER_QUAD; i += 4)
  {
    auto uv_y = uvs[i];
    uv_y *= src_rect[3] / sprite.getTexture()->getHeight();
    uv_y += src_rect[1] / sprite.getTexture()->getHeight();

    uvs[i] = uv_y;
  }

  if (sprite.isFlippedOnX() || sprite.isFlippedOnXY())
  {
    std::swap(uvs[0], uvs[12]);
    std::swap(uvs[4], uvs[8]);
  }

  if (sprite.isFlippedOnY() || sprite.isFlippedOnXY())
  {
    std::swap(uvs[1], uvs[5]);
    std::swap(uvs[9], uvs[13]);
  }
}

void ASGE::CGLSpriteRenderer::createCharQuad(
  const ASGE::GLCharRender& character, const ASGE::Colour& colour, ASGE::GPUQuad& quad) const
{
  // locate the character and set x,y positions
  const auto& ch = character.font->getAtlas()->getCharacter(character.ch);
  float x_pos    = character.x + ch.Bearing.x * character.scale;
  float y_pos    = character.y - ch.Bearing.y * character.scale;

  // generate the matrix
  auto& model_matrix = quad.position;
  model_matrix       = { glm::mat4(1.F) };

  model_matrix = glm::translate(model_matrix, glm::vec3(x_pos, y_pos, 0.0F));

  GLfloat w = ch.Size.x * character.scale;
  GLfloat h = ch.Size.y * character.scale;

  model_matrix = glm::scale(model_matrix, glm::vec3(w, h,1.0F));

  // Calculate texture coords for each character
  quad.uv_data[0] = glm::vec4{ (float)ch.UV.x, (float)ch.UV.w, GPUQuad::PADDING }; // v1
  quad.uv_data[1] = glm::vec4{ (float)ch.UV.x, (float)ch.UV.y, GPUQuad::PADDING }; // v2
  quad.uv_data[2] = glm::vec4{ (float)ch.UV.z, (float)ch.UV.y, GPUQuad::PADDING }; // v3
  quad.uv_data[3] = glm::vec4{ (float)ch.UV.z, (float)ch.UV.w, GPUQuad::PADDING }; // v4

  // generate colour data
  quad.color = { colour.r, colour.g, colour.b, character.alpha };
}

unsigned int ASGE::CGLSpriteRenderer::getBasicSpriteShaderID() const noexcept
{
  return basic_sprite_shader;
}

void ASGE::CGLSpriteRenderer::quadGen(const ASGE::GLSprite& sprite, ASGE::GPUQuad& dest) noexcept
{
  auto* gpu_model_data  = reinterpret_cast<glm::mat4*>(&dest.position);
  auto* gpu_colour_data = reinterpret_cast<glm::vec4*>(&dest.color);
  auto* gpu_uv_data     = reinterpret_cast<float*>(&dest.uv_data[0]);

  generateSpriteMatrixData(sprite, gpu_model_data);
  generateColourData(sprite, gpu_colour_data);
  generateUvData(sprite, &gpu_uv_data[0]);
}

void ASGE::CGLSpriteRenderer::setActiveShader(ASGE::SHADER_LIB::GLShader* shader)
{
  active_shader = shader;
}

ASGE::SHADER_LIB::GLShader* ASGE::CGLSpriteRenderer::activeShader()
{
  return active_shader;
}

void ASGE::CGLSpriteRenderer::apply(ASGE::RenderState* state)
{
  if(active_render_state != state)
  {
    if(!active_render_state || active_render_state->projection != state->projection)
    {
      glBindBuffer(GL_UNIFORM_BUFFER, shader_data_location);
      glBufferSubData(
        GL_UNIFORM_BUFFER,
        0,
        sizeof(glm::mat4),
        glm::value_ptr(state->projection));
    }

    if(!active_render_state || active_render_state->viewport != state->viewport)
    {
      auto& vp = state->viewport;
      glViewport(vp.x, vp.y, vp.w, vp.h);
    }
    active_render_state = state;
  }
}

void ASGE::CGLSpriteRenderer::setupGlobalShaderData()
{
  // setup shader data location
  glGenBuffers(1, &shader_data_location);
  glBindBuffer(GL_UNIFORM_BUFFER, shader_data_location);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(SHADER_DATA), nullptr, GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_UNIFORM_BUFFER, GLRenderConstants::PROJECTION_UBO_BIND, shader_data_location);
}

void ASGE::CGLSpriteRenderer::clearActiveRenderState()
{
  active_render_state = nullptr;
}
