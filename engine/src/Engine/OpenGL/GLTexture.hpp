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

#pragma once
#include "GLPixelBuffer.hpp"
#include "Texture.hpp"
#include <map>
#include <memory>

namespace ASGE
{
  class GLTexture : public Texture2D
  {
   public:
    inline static const std::map<ASGE::Texture2D::MagFilter, GLuint> GL_MAG_LOOKUP {
      std::make_pair(ASGE::Texture2D::MagFilter::LINEAR, GL_LINEAR),
      std::make_pair(ASGE::Texture2D::MagFilter::NEAREST, GL_NEAREST)
    };

    inline static const std::map<ASGE::Texture2D::MinFilter, GLuint> GL_MIN_LOOKUP {
      std::make_pair(ASGE::Texture2D::MinFilter::LINEAR, GL_LINEAR),
      std::make_pair(ASGE::Texture2D::MinFilter::LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST),
      std::make_pair(ASGE::Texture2D::MinFilter::LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR),
      std::make_pair(ASGE::Texture2D::MinFilter::NEAREST, GL_NEAREST),
      std::make_pair(ASGE::Texture2D::MinFilter::NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR),
      std::make_pair(ASGE::Texture2D::MinFilter::NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST)
    };

   private:
    inline static const std::map<ASGE::Texture2D::UVWrapMode, GLint> GL_UVWRAP_LOOKUP {
      std::make_pair(ASGE::Texture2D::UVWrapMode::CLAMP, GL_CLAMP_TO_EDGE),
      std::make_pair(ASGE::Texture2D::UVWrapMode::REPEAT, GL_REPEAT),
      std::make_pair(ASGE::Texture2D::UVWrapMode::MIRRORED, GL_MIRRORED_REPEAT)
    };

   public:
    GLTexture(int width, int height);
    ~GLTexture() override;
    GLTexture(const GLTexture&) = delete;
    GLTexture operator=(const GLTexture&) = delete;

    const unsigned int& getID() const;
    unsigned int& getID();

    PixelBuffer* getPixelBuffer() noexcept override;
    const PixelBuffer* getPixelBuffer() const noexcept override;
    void updateMinFilter(MinFilter filter) override;
    void updateMagFilter(MagFilter filter) override;
    void updateMips() override;
    void updateUVWrapping(Texture2D::UVWrapMode s, Texture2D::UVWrapMode t) override;

   private:
    bool unload();
    unsigned int id{};
    mutable std::unique_ptr<GLPixelBuffer> buffer = nullptr;
  };
}
