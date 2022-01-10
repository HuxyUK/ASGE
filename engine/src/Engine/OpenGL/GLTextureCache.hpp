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

#pragma once

#include <map>
#include <memory>
#include <string>
#include "GLTexture.hpp"
#include "NonCopyable.hpp"

namespace ASGE
{
  class GLRenderer;
  class GLTextureCache final : public NonCopyable
  {
   public:
    friend class GLRenderer;
    GLTextureCache(const GLTextureCache&) = delete;
    GLTextureCache operator=(const GLTextureCache&) = delete;
    static GLTextureCache& getInstance()
    {
      static GLTextureCache instance;
      return instance;
    }

    ASGE::GLTexture* createCached(const std::string& path);
    ASGE::GLTexture* createNonCached(int img_width, int img_height, Texture2D::Format format, void* data);
    ASGE::GLTexture* createNonCached(const std::string& path);
    ASGE::GLTexture* createNonCachedMSAA(int img_width, int img_height, Texture2D::Format format);
    ASGE::GLTexture* createCached(const std::string& uid, int img_width, int img_height, GLTexture::Format format, void* data);
    void reset();

   private:
    GLTextureCache() = default;
    ~GLTextureCache();

    ASGE::GLTexture* allocateMSAATexture(int img_width, int img_height, Texture2D::Format format);
    ASGE::GLTexture* allocateTextureArray(int img_width, int img_height, Texture2D::Format format, const void* data, int count);
    ASGE::GLTexture* allocateTexture(int img_width, int img_height, Texture2D::Format format, const void* data);
    ASGE::GLTexture* allocateTexture(const std::string& file);

    // the cache and renderer to use
		std::map<const std::string, std::unique_ptr<GLTexture>> cache;
    ASGE::GLRenderer* renderer {nullptr};
  };
}  // namespace ASGE