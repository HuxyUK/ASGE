//  Copyright (c) 2021 James Huxtable. All rights reserved.
//  Adapted from the https://learnopengl.com tutorials and code.
//  This work is licensed under the terms of the MIT license.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#include <Viewport.hpp>
#include "GLSprite.hpp"
#include "GLTextureCache.hpp"
#include "Logger.hpp"
#include "Tile.hpp"

bool ASGE::GLSprite::loadTexture(const std::string& file, AttachMode mode)
{
	texture = GLTextureCache::getInstance().createCached(file);
	if (texture != nullptr)
  {
    attach(mode);
    return true;
  }

  texture = GLTextureCache::getInstance().createCached("__asge__debug__texture__");
	return false;
}

void ASGE::GLSprite::attach(ASGE::Sprite::AttachMode mode)
{
  using AttachMode = ASGE::Sprite::AttachMode;
  if ((mode & AttachMode::KEEP_UVS) != AttachMode::KEEP_UVS)
  {
    srcRect()[0] = 0;
    srcRect()[1] = 0;
    srcRect()[2] = texture->getWidth();
    srcRect()[3] = texture->getHeight();
  }

  if ((mode & AttachMode::KEEP_DIMS) != AttachMode::KEEP_DIMS)
  {
    dimensions()[0] = texture->getWidth();
    dimensions()[1] = texture->getHeight();
  }

  if ((mode & AttachMode::KEEP_ROTATION) != AttachMode::KEEP_ROTATION)
  {
    rotationInRadians(0.0F);
  }

  if ((mode & AttachMode::KEEP_TINT) != AttachMode::KEEP_TINT)
  {
    colour(COLOURS::WHITE);
  }

  if ((mode & AttachMode::GENERATE_MIPS) == AttachMode::GENERATE_MIPS)
  {
    texture->updateMips();
  }
}

ASGE::Texture2D* ASGE::GLSprite::getTexture() const
{
  return texture;
}

bool ASGE::GLSprite::attach(ASGE::Texture2D* texture_to_attach, AttachMode mode) noexcept
{
  auto* as_gl_texture = dynamic_cast<ASGE::GLTexture*> (texture_to_attach);
  if(as_gl_texture == nullptr)
  {
    return false;
  }

  this->texture = as_gl_texture;
  attach(mode);
  return true;
}

const ASGE::GLTexture* ASGE::GLSprite::asGLTexture() const noexcept
{
  return texture;
}

const ASGE::SHADER_LIB::GLShader* ASGE::GLSprite::asGLShader() const
{
  const SHADER_LIB::GLShader* gl_shader = nullptr;
  if(this->getPixelShader() != nullptr)
  {
    gl_shader = dynamic_cast<const SHADER_LIB::GLShader*>(this->getPixelShader());

    if (gl_shader == nullptr)
    {
      Logging::ERRORS("ASGE::GLSprite::asGLShader()");
      Logging::ERRORS("Shader could not be converted to OGL");
      Logging::ERRORS("Are you sure this is a GL shader?");
    }
  }

  return gl_shader;
}

ASGE::GLSprite::GLSprite()
{
  Sprite::loadTexture("__asge__debug__texture__");
  ASGE::Viewport viewport;
  glGetIntegerv(GL_VIEWPORT, &viewport.x);
    auto ratio = std::max(
    static_cast<float>(viewport.w) / 1920.F,
    static_cast<float>(viewport.h) / 1080.F);
  width(50 * ratio);
  height(50 * ratio);
}

ASGE::GLSprite::GLSprite(const ASGE::Tile& tile) :
  texture(dynamic_cast<GLTexture*>(tile.texture))
{
  this->opacity(tile.opacity);
  this->dimensions()[0] = static_cast<float>(tile.width);
  this->dimensions()[1] = static_cast<float>(tile.height);
  this->setGlobalZOrder(tile.z);
  this->colour(tile.tint);
  this->rotationInRadians(tile.rotation);
  std::copy(tile.src_rect.begin(), tile.src_rect.end(), this->srcRect());

  if(texture == nullptr)
  {
    Sprite::loadTexture("__asge__debug__texture__");
  }
}
