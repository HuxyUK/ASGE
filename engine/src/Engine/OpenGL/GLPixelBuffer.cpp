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

#include "GLPixelBuffer.hpp"
#include "GLFormat.hpp"
#include "GLTexture.hpp"
#include <cstring>
#include <math.h>

ASGE::GLPixelBuffer::GLPixelBuffer(const GLTexture& texture)
  : PixelBuffer(
    static_cast<unsigned int>(texture.getWidth()),
    static_cast<unsigned int>(texture.getHeight())),
    pbo_read_id(0),
    format(texture.getFormat()),
    texture_id(texture.getID()),
    pixels(new GLubyte[inBytes(0)])
{
  GLVMSG(__PRETTY_FUNCTION__, glGenBuffers, 1, &pbo_read_id);
  glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_read_id);
  glBufferData(GL_PIXEL_PACK_BUFFER, inBytes(0), nullptr, GL_STREAM_READ);
  ASGE::GLPixelBuffer::download(0);
}

ASGE::GLPixelBuffer::GLPixelBuffer(ASGE::GLPixelBuffer&& rhs) noexcept
  : ASGE::PixelBuffer(rhs.getWidth(), rhs.getHeight()),
    pbo_read_id(std::exchange(rhs.pbo_read_id, 0)),
    format(rhs.format),
    texture_id(std::exchange(rhs.texture_id, 0)),
    pixels(std::move(rhs.pixels))
{
}

ASGE::GLPixelBuffer::~GLPixelBuffer()
{
  glDeleteBuffers(1, &pbo_read_id);
}

void ASGE::GLPixelBuffer::upload(unsigned int mip_level) noexcept
{
  int idx = 0;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &idx);

  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D,
               mip_level,
               GL_RGBA,
               getMipWidth(mip_level),
               getMipHeight(mip_level),
               0,
    GLFORMAT[format],
               GL_UNSIGNED_BYTE,
               pixels.get());

  if (mip_level == 0)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  glBindTexture(GL_TEXTURE_2D, idx);
}

void ASGE::GLPixelBuffer::upload(std::byte* data, unsigned int mip_level) noexcept
{
  memcpy(pixels.get(), data, inBytes(mip_level));
  upload(mip_level);
}

void ASGE::GLPixelBuffer::download(unsigned int mip_level) noexcept
{
  glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_read_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  // transfer texture into PBO
  GLVMSG(
    __PRETTY_FUNCTION__,
    glGetTexImage,
    GL_TEXTURE_2D,
    mip_level,
    GLFORMAT[format],
    GL_UNSIGNED_BYTE,
    (GLvoid*)0
    // offset in bytes into "buffer",
    // not pointer to client memory!
  );

  stale = true;
}

std::uint32_t ASGE::GLPixelBuffer::inBytes(uint32_t mip_level) const noexcept
{
  return getMipWidth(mip_level) * getMipHeight(mip_level) * format;
}

const std::byte* ASGE::GLPixelBuffer::getPixelData() const noexcept
{
  if(isBufferStale())
  {
    auto *gpu_data = glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    memcpy(pixels.get(), gpu_data, inBytes(0));
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    stale = false;
  }

  return   //NOLINTNEXTLINE
  reinterpret_cast<std::byte* const>(pixels.get());
}

std::byte* ASGE::GLPixelBuffer::getPixelData() noexcept
{
  return   //NOLINTNEXTLINE
  const_cast<std::byte*>(std::as_const(*this).getPixelData());
}

std::uint32_t ASGE::GLPixelBuffer::getMipWidth(uint32_t mip_level) const noexcept
{
  return
  static_cast<std::uint32_t>(
  mip_level == 0 ? getWidth()  : getWidth() / pow(2, mip_level));
}

std::uint32_t ASGE::GLPixelBuffer::getMipHeight(uint32_t mip_level) const noexcept
{
  return
  static_cast<std::uint32_t>(
    mip_level == 0 ? getHeight() : getHeight() / pow(2, mip_level));
}

std::uint32_t ASGE::GLPixelBuffer::pixelFormat() const noexcept
{
  return format;
}