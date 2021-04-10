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

//! @file PixelBuffer.hpp
//! @brief Class @ref ASGE::PixelBuffer

#ifndef ASGE_PIXELBUFFER_HPP
#define ASGE_PIXELBUFFER_HPP

#include <array>
#include <atomic>

namespace ASGE
{
  /**
   *
   */
  class PixelBuffer
  {
   public:

    /**
     * @brief A pixel buffer is a handle to a texture's pixel data.
     *
     * This abstract class can be used to directly download pixel
     * data from the GPU or to modify the pixels on the CPU before
     * then updating the GPU data using the pixel buffer. Modifying
     * the contents of the CPU-side of the buffer will result in the
     * PixelBuffer being out of sync with the GPU. In order to
     * synchronise the CPU and GPU, an upload function is also
     * provided. To synchronise the GPU and the CPU, a download can
     * be scheduled.
     *
     * @param[in] width The width of the buffer.
     * @param[in] height The height of the buffer.
     *
     * <example>
     * @code
     *   auto* sprite = renderer->createRawSprite();
     *   sprite->width(200);
     *   sprite->height(200);
     *
     *   auto* blank_texture = renderer->createNonCachedTexture(200, 200, ASGE::Texture2D::Format::RGBA, nullptr);
     *   auto *pixel_buffer  = blank_texture->getPixelBuffer();
     *   pixel_buffer->download(0);
     *
     *   for (int i = 0; i < pixel_buffer->getWidth() * pixel_buffer->getHeight() * 4; ++i)
     *   {
     *     static_cast<std::byte*>(pixel_buffer->getPixelData())[i] =
     *     static_cast<std::byte>((unsigned int)(std::rand() % 256));
     *   }
     *
     *   pixel_buffer->upload(0);
     *   sprite->attach(blank_texture);
     * @endcode
     *
     * @image html "pixelbuffer.png" "Resultant random noise generation" width=200
     * </example>
     */
    PixelBuffer(unsigned int width, unsigned int height): buffer_size{ width, height } {}
    virtual ~PixelBuffer() = default;

    /*
     * Move constructor.
     */
    PixelBuffer(PixelBuffer&& rhs) noexcept
    : buffer_size({rhs.getWidth(), rhs.getHeight()}), stale(rhs.isBufferStale())
    {

    }

    /*
     * Move assignment operator.
     */
    PixelBuffer& operator  =(PixelBuffer&& rhs) noexcept
    {
      this->buffer_size = rhs.buffer_size;
      this->stale       = rhs.isBufferStale();
      return *this;
    }

    /**
     * @brief Retrieves the pixel data as a pointer to an array of bytes.
     *
     * The returned pointer is to the first byte in the pixel buffer
     * array. The size of the buffer can be easily retrieved using
     * the width and height functions. Each pixel's size is determined
     * by the texture's BPP.
     *
     * @warning Accessing a pixel outside of the allocated buffer will
     * lead to undefined behaviour. You should ensure all access is
     * within the height * width * bpp bounds of the pixel buffer.
     *
     * @return A pointer to the first pixel (constant) in the buffer.
     */
    virtual const std::byte* getPixelData() const noexcept = 0;

    /**
     * @brief Retrieves the pixel data as a pointer to an array of bytes.
     *
     * The returned pointer is to the first byte in the pixel buffer
     * array. The size of the buffer can be easily retrieved using
     * the width and height functions. Each pixel's size is determined
     * by the texture's BPP.
     *
     * @warning Accessing a pixel outside of the allocated buffer will
     * lead to undefined behaviour. You should ensure all access is
     * within the height * width * bpp bounds of the pixel buffer.
     *
     * @return A pointer to the first pixel in the buffer.
     */
    virtual std::byte* getPixelData() noexcept = 0;

    /**
     * @brief Signals an upload to the GPU to update the pixels.
     * @param[in] data A new range of pixels to upload.
     * @param[in] mip_level The mip level to update.
     */
    virtual void upload(std::byte* data, unsigned int mip_level) noexcept = 0;

    /**
     * @brief Signals an upload to the GPU to update the pixels.
     * @param[in] mip_level The mip level to update.
     */
    virtual void upload(unsigned int mip_level) noexcept = 0;

    /**
     * @brief Schedules a download from the GPU.
     * @param mip_level The mip level to download.
     */
    virtual void download(unsigned int mip_level) noexcept = 0;

    /**
     * @brief Checks to see if the buffer has changed but not sync'd
     * @return Is the CPU and GPU out of sync.
     */
    bool isBufferStale() const noexcept { return stale; }

    /**
     * @brief Retrieves the width of the buffer.
     * @return The buffer's width.
     */
    unsigned int getWidth() const noexcept { return buffer_size[0]; }

    /**
     * @brief Retrieves the height of the buffer.
     * @return The buffer's height.
     */
    unsigned int getHeight() const noexcept { return buffer_size[1]; }

   private:
    std::array<unsigned int, 2> buffer_size{ 0 };

   protected:
    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
    mutable std::atomic<bool> stale{ true };
  };
} // namespace ASGE

#endif // ASGE_PIXELBUFFER_HPP
