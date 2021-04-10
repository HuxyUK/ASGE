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

#include "GLSprite.hpp"
#include "GLTextureCache.hpp"

bool ASGE::GLSprite::loadTexture(const std::string& file)
{
	texture = GLTextureCache::getInstance().createCached(file);
	if (texture != nullptr)
	{
		// sane defaults
		dimensions()[0] = texture->getWidth();
    dimensions()[1] = texture->getHeight();

		// source rectangle
		srcRect()[2] = dimensions()[0];
		srcRect()[3] = dimensions()[1];
		return true;
	}

	return false;
}

ASGE::Texture2D* ASGE::GLSprite::getTexture() const
{
  return texture;
}

bool ASGE::GLSprite::attach(ASGE::Texture2D* texture_to_attach) noexcept
{
  auto* correct_type = dynamic_cast<ASGE::GLTexture*> (texture_to_attach);
  if(correct_type == nullptr)
  {
    return false;
  }

  this->texture = correct_type;
  srcRect()[2] = texture->getWidth();
  srcRect()[3] = texture->getHeight();
  this->texture->updateMips();
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
