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

#ifndef ASGE_GLPIXELBUFFER_HPP
#define ASGE_GLPIXELBUFFER_HPP
#include "GLIncludes.hpp"
#include "NonCopyable.hpp"
#include "PixelBuffer.hpp"
#include <memory>

namespace ASGE
{
  class GLTexture;
}

namespace ASGE
{
  class GLPixelBuffer : public ASGE::PixelBuffer
  {
   public:
    explicit GLPixelBuffer(const GLTexture& texture);
    ~GLPixelBuffer() override;

    GLPixelBuffer(const GLPixelBuffer& rhs) = delete;
    GLPixelBuffer(GLPixelBuffer&& rhs) noexcept;
    GLPixelBuffer& operator=(const GLPixelBuffer& rhs) = delete;
    GLPixelBuffer& operator=(GLPixelBuffer&& rhs) = delete;

    const std::byte* getPixelData() const noexcept override;
    std::byte* getPixelData() noexcept override;
    void upload(std::byte*, unsigned int mip_level) noexcept override;
    void download(unsigned int mip_level) noexcept override;
    void upload(unsigned int mip_level) noexcept override;
    std::uint32_t pixelFormat() const noexcept;

   private:
    std::uint32_t inBytes(uint32_t mip_level) const noexcept;
    std::uint32_t getMipWidth(uint32_t mip_level) const noexcept;
    std::uint32_t getMipHeight(uint32_t mip_level) const noexcept;

   private:
    GLuint pbo_read_id;
    GLuint format;
    GLuint texture_id;
    std::unique_ptr<GLubyte[]> pixels;
  };
}

#endif // ASGE_GLPIXELBUFFER_HPP
