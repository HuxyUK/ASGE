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

#include "GLTexture.hpp"
#include "GLFormat.hpp"
#include "GLIncludes.hpp"
#include "GLPixelBuffer.hpp"
#include <cassert>

ASGE::GLTexture::GLTexture(int width, int height) : Texture2D(width, height) {}

ASGE::GLTexture::~GLTexture()
{
	unload();
}

bool ASGE::GLTexture::unload()
{
	glDeleteTextures(1, &id);
	return false;
}

const unsigned int& ASGE::GLTexture::getID() const
{
  return id;
}

unsigned int& ASGE::GLTexture::getID()
{
  return id;
}

ASGE::PixelBuffer* ASGE::GLTexture::getPixelBuffer() noexcept
{
  if(buffer)
  {
    return buffer.get();
  }

  buffer = std::make_unique<GLPixelBuffer>(*this);
  return buffer.get();
}

const ASGE::PixelBuffer* ASGE::GLTexture::getPixelBuffer() const noexcept
{
  if(buffer)
  {
    return buffer.get();
  }

  buffer = std::make_unique<GLPixelBuffer>(*this);
  return buffer.get();
}

void ASGE::GLTexture::updateMagFilter(ASGE::Texture2D::MagFilter filter)
{
  if (id == 0U)
  {
    Logging::WARN(__PRETTY_FUNCTION__);
    Logging::WARN("Trying to update filtering level on invalid texture");
    return;
  }

  GLVCMD(glActiveTexture, GL_TEXTURE0);
  GLVCMD(glBindTexture ,GL_TEXTURE_2D,getID());
  GLVMSG(
    "Setting Mag Filter",
    glTexParameteri,
    GL_TEXTURE_2D,
    GL_TEXTURE_MAG_FILTER,
    GLTexture::GL_MAG_LOOKUP.at(filter));
}

void ASGE::GLTexture::updateMips()
{
  if (id == 0U)
  {
    Logging::WARN(__PRETTY_FUNCTION__);
    Logging::WARN("Trying to update filtering level on invalid texture");
    return;
  }

  GLVCMD(glActiveTexture, GL_TEXTURE0);
  GLVCMD(glBindTexture, GL_TEXTURE_2D, getID());
  GLVMSG("Rebuilding Mips", glGenerateMipmap, GL_TEXTURE_2D);
}
