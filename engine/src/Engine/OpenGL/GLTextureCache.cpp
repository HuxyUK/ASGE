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

// STB
#define STB_IMAGE_IMPLEMENTATION
#include <cmath>
#include <memory>
#include <stb_image.h>

#include "FileIO.hpp"
#include "Logger.hpp"
#include "GLFormat.hpp"
#include "GLIncludes.hpp"
#include "GLRenderer.hpp"
#include "GLTexture.hpp"
#include "GLTextureCache.hpp"

ASGE::GLTextureCache::~GLTextureCache()
{
	reset();
}

void ASGE::GLTextureCache::reset()
{
	for (auto& texture : cache)
	{
		if (texture.second)
		{
			texture.second = nullptr;
		}
	}
	cache.clear();
}

ASGE::GLTexture* ASGE::GLTextureCache::createCached(const std::string& path)
{
  auto& cached_texture = cache[path];
  if (cached_texture)
  {
    return cached_texture.get();
  }

  cache[path] = std::unique_ptr<ASGE::GLTexture> { allocateTexture(path) };
  return cache[path].get();
}

ASGE::GLTexture* ASGE::GLTextureCache::createCached(
  const std::string& id, int img_width, int img_height, GLTexture::Format format, void* data)
{
  if (id.empty())
  {
    return nullptr;
  }

  auto& cached_texture = cache[id];
  if (cached_texture)
  {
    return cached_texture.get();
  }

  cache[id] = std::unique_ptr<ASGE::GLTexture>{ allocateTexture(img_width, img_height, format, data) };
  return cache[id].get();
}

ASGE::GLTexture* ASGE::GLTextureCache::createNonCached(const std::string& path)
{
  return allocateTexture(path);
}

ASGE::GLTexture* ASGE::GLTextureCache::createNonCached(
  int img_width, int img_height, Texture2D::Format format, void* data)
{
  return allocateTexture(img_width, img_height, format, data);
}

ASGE::GLTexture* ASGE::GLTextureCache::createNonCachedMSAA(int img_width, int img_height, Texture2D::Format format)
{
  return allocateMSAATexture(img_width, img_height, format);
}

ASGE::GLTexture* ASGE::GLTextureCache::allocateTexture(const std::string& file)
{
  int   img_width  = 0;
  int   img_height = 0;
  int   bpp        = 0;
  void* image      = nullptr;

  auto binary = ASGE::FILEIO::File();
  if(binary.open(file, ASGE::FILEIO::File::IOMode::READ))
  {
    ASGE::FILEIO::IOBuffer buffer = binary.read();

    image = stbi_load_from_memory(
      buffer.as_unsigned_char(),
      static_cast<int>(buffer.length),
      &img_width,
      &img_height,
      &bpp,
      STBI_default);
  }
  else
  {
    image = stbi_load(file.c_str(), &img_width, &img_height, &bpp, STBI_default);
  }

  if (image == nullptr)
  {
    Logging::ERRORS(file + " failed to load");
    return nullptr;
  }

  auto format = ASGE::Texture2D::Format{ static_cast<ASGE::Texture2D::Format>(bpp) };
  auto* texture = allocateTexture(img_width, img_height, format, image);

  stbi_image_free(image);
  return texture;
}

ASGE::GLTexture*
ASGE::GLTextureCache::allocateTexture(
  int img_width, int img_height, Texture2D::Format format, const void* data)
{
  auto *texture = new GLTexture(img_width, img_height);
  texture->setFormat(format);

  // Allocate a texture
  glGenTextures(1, &texture->getID());
  glBindTexture(GL_TEXTURE_2D, texture->getID());

  // Load the 2D texture
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    img_width, img_height,
    0,
    GLFORMAT[texture->getFormat()],
    GL_UNSIGNED_BYTE,
    data);

  // Set texture options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // Use the default game settings for mag filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GLTexture::GL_MAG_LOOKUP.at(renderer->magFilter()));

  if(data != nullptr)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  if (ASGE::GLRenderer::RENDER_LIB == ASGE::GLRenderer::RenderLib::GL_MODERN)
  {
    // Anisotropic filtering
    float aniso_level = 16;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso_level);
    //aniso_level = std::min(float(renderer->ansio()), aniso_level);
    glTextureParameterf(texture->getID(), GL_TEXTURE_MAX_ANISOTROPY, aniso_level);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}

ASGE::GLTexture* ASGE::GLTextureCache::allocateMSAATexture(int img_width, int img_height, Texture2D::Format format)
{
  auto *texture = new GLTexture(img_width, img_height);
  texture->setFormat(format);

  // Allocate a texture
  glGenTextures(1, &texture->getID());
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture->getID());
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, renderer->msaa(), GLFORMAT[texture->getFormat()], img_width, img_height, GL_TRUE);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
  return texture;
}

ASGE::GLTexture* ASGE::GLTextureCache::allocateTextureArray(
  int img_width, int img_height, ASGE::Texture2D::Format format, const void* data, int count)
{
  auto *texture = new GLTexture(img_width, img_height);
  texture->setFormat(format);

  glGenTextures(1, &texture->getID());
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture->getID());

  // Load the 2D texture
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, img_width, img_height, count);

  // Upload pixel data
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                  0, 0, 0, 0, img_width, img_height, count, GLFORMAT[texture->getFormat()],
                  GL_UNSIGNED_BYTE, data);

  // Set texture options
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // Use the default game settings for mag filtering
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER,
                  GLTexture::GL_MAG_LOOKUP.at(renderer->magFilter()));

  if(data != nullptr)
  {
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  }

  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

  ASGE::ClearGLErrors("Error: Allocating texture array!");
  return texture;
}
