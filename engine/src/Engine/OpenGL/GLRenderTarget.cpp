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

#include "GLRenderTarget.hpp"
#include "GLRenderer.hpp"
#include "GLTexture.hpp"
ASGE::GLRenderTarget::GLRenderTarget(
  ASGE::Renderer* renderer, int width, int height, ASGE::Texture2D::Format format, int count)
{
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  // The texture buffers
  buffers.reserve(count);
  std::vector<GLenum> draw_buffers{};
  for (int i = 0; i < count; ++i)
  {
    auto* texture = renderer->createNonCachedTexture(width, height, format, nullptr);
    auto* buffer  = buffers.emplace_back(dynamic_cast<ASGE::GLTexture*>(texture));
    if (buffer == nullptr)
    {
      Logging::ERRORS("Trying to attach a texture to framebuffer which is not valid");
      continue;
    }
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, buffer->getID(), 0);
    draw_buffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
  }

  // Specifies a list of color buffers to be drawn into
  glDrawBuffers(draw_buffers.size(), draw_buffers.data());

  // The depth buffer
  glGenRenderbuffers(1, &DBO);
  glBindRenderbuffer(GL_RENDERBUFFER, DBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DBO);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    Logging::ERRORS("Attempt to create a valid FrameBuffer has failed");
  }
}

ASGE::GLRenderTarget::~GLRenderTarget()
{
  glDeleteRenderbuffers(1, &DBO);
  glDeleteFramebuffers(1, &FBO);
}

void ASGE::GLRenderTarget::use() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

const std::vector<ASGE::GLTexture*>& ASGE::GLRenderTarget::getBuffers() const
{
  return buffers;
}

void ASGE::GLRenderTarget::setBuffers(const std::vector<ASGE::GLTexture*>& buffers)
{
  GLRenderTarget::buffers = buffers;
}
