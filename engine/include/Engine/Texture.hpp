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

//! @file Texture.hpp
//! @brief Class @ref ASGE::Texture2D, Enum Class @ref ASGE::SpriteSortMode

#pragma once
#include "GameSettings.hpp"
#include "NonCopyable.hpp"
#include "PixelBuffer.hpp"
namespace ASGE
{
	/**
	* @brief modes to control the batching of sprites
	*
	* Used to control the sprite batching techniques
	* that renderers may support.
	*/
	enum class SpriteSortMode
	{
		IMMEDIATE,     /**< Renders the sprite immediately.
					              Invokes a state change, uploads to the gpu and draws the
							          primitives. This can be an expensive operation. */
		TEXTURE,       /**< Prepares the render data but does not populate the shared buffers
					              nor does it render immediately. Instead it queues all the draw calls
							          and then sorts them by their texture id. This reduces texture and
							          GPU state changes, but breaks the original draw order and is slightly
							          more intensive on the CPU and memory systems. */
		BACK_TO_FRONT, /**< Works identically to texture, but also uses the z-order to control the
					              order of draw calls. All draw calls will be first sorted by their z-order
							          and then sorted by their texture id. This mode will render the sprites with
							          the lowest z-order first. */
		FRONT_TO_BACK, /**< Works identically to texture, but also uses the z-order to control the
					              order of draw calls. All draw calls will be first sorted by their z-order
							          and then sorted by their texture id. This mode will render the sprites with
							          the highest z-order first. */
	};

  /**
	* @brief a simple 2D non-renderable texture.
	* 
	* A texture is used to load images into the GPU.
	* They retain a handle or link to it and the 
	* renderers are free to implement any caching systems
	* they see fit. 
	*/
	class Texture2D
	{
	public:

    /**< Shorthand for MagFilter */
    using MagFilter = GameSettings::MagFilter;

    /**
    * The format of the texture
    */
    enum Format
    {
        MONOCHROME = 1, /**< The texture is monochromatic.*/
        MONOCHROME_ALPHA = 2, /**< The texture is monochromatic and has an alpha channel.*/
        RGB = 3, /**< There are Red, Green & Blue channels present.*/
        RGBA = 4 /**< There are RGB plus alpha channels present .*/
    };

    /**
     * Min filters used when scaling textures downwards
     */
    enum class MinFilter : uint32_t
    {
      LINEAR ,
      LINEAR_MIPMAP_NEAREST,
      LINEAR_MIPMAP_LINEAR,
      NEAREST,
      NEAREST_MIPMAP_LINEAR,
      NEAREST_MIPMAP_NEAREST
    };


    /**
     * @brief modes that controlling the UV texture wrapping
     *
     * When sampling textures and using UV coordinates outside of
     * the normalised range, wrapping modes can be used to control
     * the resultant sampled pixel. For example, REPEAT will
     * continuously wrap the texture. This is good for when you want
     * to sample outside the normal [0,1] range but still obtain a
     * resultant pixel. A use case for this is a scrolling background layer.
     */
    enum class UVWrapMode
    {
      CLAMP,   /**< Clamps the texture to [0,1].        */
      REPEAT,  /**< Repeats the texture.                */
      MIRRORED /**< Mirrors the image with each repeat. */
    };

    /**
		* Default constructor.
		* @param width The width of the texture.
		* @param height the height of the texture.
		*/
		Texture2D(int width, int height) noexcept
		{
			dims[0] = static_cast<float>(width);
			dims[1] = static_cast<float>(height);
		}
			
		/**
		* Default destructor.
		* The destructor does not free the memory used on the GPU.
		* **This is handled inside the platform's specific implementation.**
		*/
		virtual ~Texture2D() = default;

 	  /**
		* Sets the format of the texture. 
		* @param f The format of the stored texture.
		* @see Format
		*/
		void setFormat(Format f) noexcept {	format = f; } 

		/**
		* Retrieves the format of the stored texture. 
		* @return The format of the texture. 
		* @see Format
		*/
		[[nodiscard]] Format getFormat() const { return format;}

    /**
		* Sets the filtering used for texture magnification.
		* Allows the type of filtering applied when
    * sampling the texture under magnification to be changed.
		* @param filter The magnification filter to apply.
		*/
    virtual void updateMagFilter(MagFilter filter) = 0;

    /**
		* Sets the filtering used for texture minification.
		* Allows the type of filtering applied when
    * sampling the texture under minification to be changed.
		* @param filter The minification filter to apply.
     */
    virtual void updateMinFilter(MinFilter filter) = 0;

    /**
     * @brief Controls how the UV coordinates are wrapped.
     *
     * When sampling the texture, UV coordinates outside of
     * the range of [0,1] can either be clamped or repeated. It
     * is also possible to clamp in one direction, and repeat in
     * another as both directions (s,t) can be controlled
     * independently of each other.
     *
     * @param s The wrapping mode for the x dimension.
     * @param t The wrapping mode for the y dimension.
     */
    virtual void updateUVWrapping(UVWrapMode s, UVWrapMode t) = 0;

    /**
    * Rebuilds the mip maps used for minification.
    */
    virtual void updateMips() = 0;

    /**
     * @brief Retrieves the texture from GPU memory.
     * Retrieving memory from the GPU like this is a slow process.
     * It is best to do this infrequently.
     * @return The binary file loaded in memory.
     */
    virtual PixelBuffer* getPixelBuffer() noexcept = 0;

    /**
     * @brief Retrieves the texture from GPU memory.
     * Retrieving memory from the GPU like this is a slow process.
     * It is best to do this infrequently.
     * @return The binary file loaded in memory.
     */
    [[nodiscard]] virtual const PixelBuffer* getPixelBuffer() const noexcept = 0;

		/**
		* Retrieves the width of the loaded texture.
		* @return The width of the texture.
		*/
		[[nodiscard]] float getWidth()  const noexcept { return dims[0]; }
		
		/**
		* Retrieves the width of the loaded texture.
		* @return The height of the texture.
		*/
		[[nodiscard]] float getHeight() const noexcept { return dims[1]; }


	private:
    //MagFilter mag_filter;   /**< Texture2D Magnification Filter. Filtering to use when magnifying the texture. */
	  Format format = RGB;		/**< Texture2D Format. The pixel format used when loading the texture file. */
		std::array<float,2>dims{ 0,0 };	/**< Texture2D Dimensions. The dimensions of the loaded texture file. */
	};
}  // namespace ASGE
