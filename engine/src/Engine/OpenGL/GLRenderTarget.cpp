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
#include "GLRenderTarget.hpp"
#include "GLRenderer.hpp"
#include "GLTexture.hpp"

ASGE::GLRenderTarget::GLRenderTarget(
  ASGE::Renderer* renderer, int width, int height, ASGE::Texture2D::Format format, int count)
{
  // https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
  glGenFramebuffers(1, &MSAA_FBO);
  glGenFramebuffers(1, &FBO);
  createFboWithAttachments(renderer, width, height, format, count);
  createFboWithMultiSampledAttachments(renderer, width, height, format, count);
}

ASGE::GLRenderTarget::~GLRenderTarget()
{
  glDeleteRenderbuffers(1, &MSAA_DBO);
  glDeleteFramebuffers(1, &MSAA_FBO);
  glDeleteFramebuffers(1, &FBO);
}

/* @brief Binds the MSAA frame buffer, ready for rendering. */
void ASGE::GLRenderTarget::use() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, MSAA_FBO);
}

/* @brief Returns the most recently resolved textures.
 *
 * Once a MSAA texture has been resolved, it will be stored in a list of resolved
 * textures. This function returns them. If the MSAA textures have been updated then
 * these will be stale and a resolve function call will be required to update them.
 *
 * @return A list of all the currently resolved textures.
 * @warning You need to call resolve to sample the buffers correctly.
 */
const std::vector<std::unique_ptr<ASGE::GLTexture>>& ASGE::GLRenderTarget::getResolved() const
{
  return resolved_textures;
}

/* @brief Creates a new FBO with MSAA support.
 *
 * In order for MSAA to work correctly, textures need to be created using specific
 * multi sampling options and the buffer attachments need to also be multi-sampled.
 * The generated FBO from this function will support both multi-sampled textures
 * and a multi-sampled depth buffer.
 *
 * @param renderer[in] The renderer to use for allocations.
 * @param width[in] The width of the fame buffer object.
 * @param height[in] The height of the frame buffer object.
 * @param format[in] The pixel format to use. Typically, RBG or RBGA.
 * @param count[in] The number of textures to attach.
 */
void ASGE::GLRenderTarget::createFboWithMultiSampledAttachments(ASGE::Renderer* renderer, int width, int height, ASGE::Texture2D::Format format, int count)
{
  glBindFramebuffer(GL_FRAMEBUFFER, MSAA_FBO);

  // The texture buffers
  msaa_textures.reserve(count);
  std::vector<GLenum> draw_buffers{};
  for (int i = 0; i < count; ++i)
  {
    auto* texture = renderer->createNonCachedMSAATexture(width, height, format);
    auto& buffer  = msaa_textures.emplace_back(dynamic_cast<ASGE::GLTexture*>(texture));
    if (buffer == nullptr)
    {
      Logging::ERRORS("Trying to attach a texture to framebuffer which is not valid");
      continue;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, buffer->getID(), 0);
    draw_buffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
  }

  // Specifies a list of color buffers to be drawn into
  glDrawBuffers(static_cast<GLsizei>(draw_buffers.size()), draw_buffers.data());

  // The depth buffer
  glGenRenderbuffers(1, &MSAA_DBO);
  glBindRenderbuffer(GL_RENDERBUFFER, MSAA_DBO);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, (renderer)->msaa(), GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, MSAA_DBO);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    Logging::ERRORS("Attempt to create a valid MSAA FrameBuffer has failed");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* @brief Creates a new FBO without MSAA support.
 *
 * In order for MSAA to work correctly when using FrameBuffer objects and normal
 * samplers, you require a resolved texture. This FBO will be used to store the
 * resolved texture.
 *
 * @param renderer[in] The renderer to use for allocations.
 * @param width[in] The width of the fame buffer object.
 * @param height[in] The height of the frame buffer object.
 * @param format[in] The pixel format to use. Typically, RBG or RBGA.
 * @param count[in] The number of textures to attach.
 */
void ASGE::GLRenderTarget::createFboWithAttachments(ASGE::Renderer* renderer, int width, int height, ASGE::Texture2D::Format format, int count)
{
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  // The texture buffers
  resolved_textures.reserve(count);
  std::vector<GLenum> draw_buffers{};
  for (int i = 0; i < count; ++i)
  {
    auto* texture = renderer->createNonCachedTexture(width, height, format, nullptr);
//    texture->updateMagFilter(ASGE::Texture2D::MagFilter::NEAREST);
    auto& buffer  = resolved_textures.emplace_back(dynamic_cast<ASGE::GLTexture*>(texture));
    if (buffer == nullptr)
    {
      Logging::ERRORS("Trying to attach a texture to framebuffer which is not valid");
      continue;
    }
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, buffer->getID(), 0);
    draw_buffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
  }

  glDrawBuffers(static_cast<GLsizei>(draw_buffers.size()), draw_buffers.data());

  // The depth buffer
  glGenRenderbuffers(1, &DBO);
  glBindRenderbuffer(GL_RENDERBUFFER, DBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DBO);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    Logging::ERRORS("Attempt to create a valid MSAA FrameBuffer has failed");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* @brief Resolves the MSAA texture at specified index.
 *
 * Before sampling can take place, the multi-sampled images need to be "blited" in to
 * a standard 2D texture. As render targets may have multiple color attachments, this
 * function allows the user to specify which attachment to specifically update.
 *
 * @param index[in] The index of the attachment to resolve. A count of 1 is index 0.
 * @return A handle to the resultant updated textures.
 * @warning You need to call resolve to sample the buffers correctly.
 */
ASGE::Texture2D* ASGE::GLRenderTarget::resolve(unsigned int index)
{
  auto *resolved_texture = resolved_textures[index].get();
  auto width = static_cast<GLint>(resolved_texture->getWidth());
  auto height = static_cast<GLint>(resolved_texture->getHeight());

  // https://stackoverflow.com/a/48125123, automatically flip on the Y AXIS
  glBindFramebuffer(GL_READ_FRAMEBUFFER, MSAA_FBO);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
  glBlitFramebuffer(0, 0, width, height, 0, height, width, 0, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  resolved_texture->updateMips();
  ASGE::ClearGLErrors();
  return resolved_texture;
}

/* @brief Resolves all MSAA textures attached to this frame buffer.
 *
 * Before sampling can take place, the multi-sampled images need to be "blited" in to
 * a standard 2D texture. As render targets may have multiple color attachments, this
 * function loops through all attachments and resolves them into textured that can then
 * be used in normal samplers in the fragment shader.
 *
 * @return A list of all the resultant updated texture.
 * @warning You need to call resolve to sample the buffers correctly.
 */
const std::vector<std::unique_ptr<ASGE::GLTexture>>& ASGE::GLRenderTarget::resolve()
{
  for(auto i = 0; i < msaa_textures.size(); ++i)
  {
    resolve(i);
  }
  return resolved_textures;
}
