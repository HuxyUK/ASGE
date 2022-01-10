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

//! @file Sprite.hpp
//! @brief
//! Class @ref ASGE::Sprite,
//! Enum @ref ASGE::Sprite::FlipFlags,
//! Enum @ref ASGE::Sprite::SourceRectIndex

#pragma once
#include "Texture.hpp"
#include "Colours.hpp"
#include "SpriteBounds.hpp"
#include <memory>
#include <string>

namespace ASGE
{
  class Renderer;
  class Texture2D;

  namespace SHADER_LIB
  {
    class Shader;
  }

  /**
   *  @brief An ASGE supported sprite.
   *
   *  Used to render and load textures. Includes the ability to
   *  resize, scale, flip, recolour etc. 2D games are made up of these.
   *  This is the interface that defines the platform specific
   *  implementation's API. The  management and loading of GPU data
   *  is handled within the inherited specialisations.
   */
  class Sprite
  {
   public:
    /**
     * @brief Flags to control the flipping of the sprite
     *
     * Used to flip the texture's UV coordinates.
     * The flip is performed on the CPU when calculating the UVs.
     */
    enum FlipFlags
    {
      NORMAL    = 0x00,           /**< texture is not flipped. */
      FLIP_X    = 0x01,           /**< texture is flipped on the x axis. */
      FLIP_Y    = 0x02,           /**< texture is flipped on the y axis. */
      FLIP_XY   = 0x04,           /**< texture is flipped diagonally. */
      FLIP_BOTH = FLIP_X | FLIP_Y /**< texture is flipped on both axis.  */
    };

    /**
     * @brief a simple enum to help with src rectangles
     * These enums provide the index numbers that are used for
     * manipulating the UV coordinates when mapping textures to
     * sprites. They form a rectangle that contents delimit the
     * range to use when calculating the source UV coordinates.
     */

    enum SourceRectIndex
    {
      SRC_START_X  = 0, /**< The starting position on the texture's X axis. */
      SRC_START_Y  = 1, /**< The starting position on the texture's Y axis. */
      SRC_LENGTH_X = 2, /**< The length of the source rectangle. */
      SRC_LENGTH_Y = 3  /**< The width of the source rectangle. */
    };

    /**
     * @brief Flags to control attachments of textures.
     * When attaching a texture to the sprite, by default the sprite's
     * settings will be reset to match the newly attached texture. These
     * flags allow the user to prevent certain resets from occurs.
     */
    enum class AttachMode : int8_t
    {
      DEFAULT   = 0x00,      /**< By default, clobber the existing sprite's settings. */
      KEEP_DIMS = 0x01,      /**< Retain the sprite's dimensions. */
      KEEP_UVS  = 0x02,      /**< Retain the sprite's UV settings. */
      KEEP_TINT = 0x04,      /**< Retain the sprite's tint. */
      KEEP_ROTATION = 0x08,  /**< Retain the sprite's rotation. */
      GENERATE_MIPS = 0x10,  /**< Generate a new set of MipMaps for the texture. */
    };

    friend AttachMode operator|(AttachMode lhs, AttachMode rhs) {
      return static_cast<AttachMode>(
          static_cast<std::underlying_type<AttachMode>::type>(lhs) |
          static_cast<std::underlying_type<AttachMode>::type>(rhs)
      );
    }

    friend AttachMode operator&(AttachMode lhs, AttachMode rhs) {
      return static_cast<AttachMode>(
          static_cast<std::underlying_type<AttachMode>::type>(lhs) &
          static_cast<std::underlying_type<AttachMode>::type>(rhs)
      );
    }

    /**
     * @brief Default destructor.
     *
     * The destructor does not free the memory used on the GPU.
     * **This is handled inside the platform's specific implementation.**
     */
    virtual ~Sprite() = default;

    /**
     * @brief Pure virtual function for loading texture on to the GPU.
     *
     * **This is handled inside the platform's specific implementation.**
     *
     * * @return The result of the load operation.
     */
    virtual bool loadTexture(const std::string&, AttachMode mode) = 0;

    /**
     * @brief
     * @return The result of the load operation.
     */
    bool loadTexture(const std::string&);


    /**
     * @brief Pure virtual function for retrieving the loaded texture.
     *
     * **This is handled inside the platform's specific implementation.**
     *
     * @return The currently attached texture.
     */
    [[nodiscard]] virtual Texture2D* getTexture() const = 0;

    /**
     * @brief Returns the sprite's position on the X axis.
     *
     * Sprite positions are described using Cartesian Coordinates. The
     * horizontal position in this engine is labelled **X** and runs
     * from left to right. If an ASGE::Camera is not used it is mapped
     * directly to the screen via the viewport.
     *
     * @return The position current position the x axis.
     */
    [[nodiscard]] float xPos() const noexcept;

    /**
     * @brief Updates the sprite's position on the X axis.
     *
     * Sprite positions are described using Cartesian Coordinates. The
     * horizontal position in this engine is labelled **X** and runs
     * from left to right. If an ASGE::Camera is not used it is mapped
     * directly to the screen via the viewport.
     *
     * @param[in] x The sprites new position on the x axis.
     */
    void xPos(float x) noexcept;

    /**
     * @brief Returns the sprite's position on the Y axis.
     *
     * Sprite positions are described using Cartesian Coordinates. The
     * vertical position in this engine is labelled **y** and runs
     * from top to bottom. If an ASGE::Camera is not used it is mapped
     * directly to the screen via the viewport.
     *
     * @return The sprites new position on the y axis */
    [[nodiscard]] float yPos() const noexcept;

    /**
     * @brief Returns the sprite's position on the Y axis.
     *
     * Sprite positions are described using Cartesian Coordinates. The
     * vertical position in this engine is labelled **y** and runs
     * from top to bottom.If an ASGE::Camera is not used it is mapped
     * directly to the screen via the viewport.
     *
     * @param[in] y The new position on the y axis */
    void yPos(float y) noexcept;

    /**
     * @brief Gets the width of the sprite.
     *
     * Width is used (along with scale, rotation etc) to determine how
     * wide/long a sprite will be when it's rendered to the screen. This
     * is the non-transformed width of the sprite.
     *
     * @return The sprites width.
     */
    [[nodiscard]] float width() const noexcept;

    /**
     * @brief Sets the width of the sprite.
     *
     * Width is used (along with scale, rotation etc) to determine how
     * wide/long a sprite will be when it's rendered to the screen. This
     * is the non-transformed width of the sprite.
     *
     * @param [in] width The sprites width.
     */
    void width(float width) noexcept;

    /**
     * @brief Gets the height of the sprite.
     *
     * Height is used (along with scale, rotation etc) to determine
     * how tall a sprite will be when it's rendered to the screen. This
     * is the non-transformed height of the sprite.
     *
     * @return The sprites height.
     */
    [[nodiscard]] float height() const noexcept;

    /**
     * @brief Sets the height of the sprite.
     *
     * Height is used (along with scale, rotation etc) to determine
     * how tall a sprite will be when it's rendered to the screen. This
     * is the non-transformed height of the sprite.
     *
     * @param [in] height The sprites height.
     */
    void height(float height) noexcept;

    /**
     * @brief Gets the non-transformed height and the width of the sprite.
     *
     * Updates the width and the height of two parameters passed in
     * to the function. It does **not** return a value. The width and
     * height is found in the resultant parameters.
     *
     * @param[in] width The sprites width.
     * @param[in] height The sprites height.
     */
    void dimensions(float& width, float& height) const noexcept;

    /**
     * @brief Gets the non-transformed height and the width of the sprite.
     * @return A tuple representing width and height.
     */
    [[nodiscard]] std::tuple<int,int> dimensions() const noexcept;

    /**
     * @brief Gets the rotation of the sprite in radians.
     *
     * The angle of rotation is used to rotate the sprite around it's
     * origin. The origin defaults to the mid-point of the sprite.
     * The angle of rotation is expressed in radians.
     *
     * @return The sprites angle of rotation in radians.
     * @note \f$(degrees = radians * 180^\theta\ / pi)\f$
     */
    [[nodiscard]] float rotationInRadians() const noexcept;

    /**
     * @brief Sets the rotation of the sprite in radians.
     *
     * The angle of rotation is used to rotate the sprite around it's
     * origin. The origin defaults to the mid-point of the sprite.
     * The angle of rotation is expressed in radians.
     *
     * @param[in] rotation_radians The angle of rotation in radians.
     * @note \f$(degrees = radians * 180^\theta\ / pi)\f$
     */
    void rotationInRadians(float rotation_radians);

    /**
     * @brief Gets the scale of the sprite.
     *
     * The scale can be adjusted to automatically make the sprite
     * bigger or smaller than it's original size, allowing for on
     * the fly alterations without having to edit the sprite's
     * true size. The scaling operation is performed in both axis.
     *
     * @return The scale factor.
     */
    [[nodiscard]] float scale() const noexcept;

    /**
     * @brief Sets the scale of the sprite.
     *
     * The scale can be adjusted to automatically make the sprite
     * bigger or smaller than it's original size, allowing for on the
     * fly alterations without having to edit the sprite's true size.
     * The scaling operation is performed in both axis.
     *
     * @param[in] scale_value The scaling factor to apply to the sprite.
     */
    void scale(float scale_value) noexcept;

    /**
     * @brief Gets the colour of the sprite.
     *
     * The colour of the sprite can be influenced by providing a tint.
     * The pixel shader will then apply the tint on the resultant pixel
     * when sampling the texture.
     *
     * @return The sprite's colour.
     * @see ASGE::Colour
     */
    [[nodiscard]] Colour colour() const noexcept;

    /**
     * @brief Sets the colour of the sprite.
     *
     * The colour of the sprite can be influenced by providing a tint.
     * The pixel shader will then apply the tint on the resultant pixel
     * when sampling the texture.
     *
     * @param[in] sprite_colour The sprite's colour.
     * @see ASGE::Colour
     * @see ASGE::COLOURS
     */
    void colour(ASGE::Colour sprite_colour) noexcept;

    /**
     * @brief Checks to see if the texture is flipped on the X axis.
     *
     * It is possible to flip the UV coordinates used on the sprite's
     * texture. This will check to see if the X axis is flipped.
     *
     * @return If the texture is flipped on the X axis.
     */
    [[nodiscard]] bool isFlippedOnX() const noexcept;

    /**
     * @brief Checks to see if the texture is flipped on the Y axis.
     *
     * It is possible to flip the UV coordinates used on the sprite's
     * texture. This will check to see if the Y axis is flipped.
     *
     * @return If the texture is flipped on the Y axis.
     */
    [[nodiscard]] bool isFlippedOnY() const noexcept;

    /**
     * @brief Checks to see if the texture is flipped diagonally.
     * @return If the texture is flipped on the Y axis.
     */
    [[nodiscard]] bool isFlippedOnXY() const noexcept;

    /**
     * @brief Sets the flip state of the texture.
     *
     * It is possible to flip the UV coordinates used on the sprite's
     * texture. This function allows setting of the flip flags.
     * These are then used to control shader's UV mappings.
     *
     * @param flags The flip flags to apply to the texture.
     */
    void setFlipFlags(FlipFlags flags) noexcept;

    /**
     * @brief Returns the current flip flags for the sprite.
     *
     * Flip flags can be used to change the orientation of the
     * texture's render. It does this by altering UV mappings.
     * Use this function to retrieve the current flip flags used
     * for rendering the attached texture.
     *
     * @return flags The flip flags that apply to the texture.
     * @see ASGE::Sprite::FlipFlags
     */
    [[nodiscard]] FlipFlags flipFlags() const noexcept;

    /**
     * @brief Sets the opacity of the sprite.
     *
     * This function let's you control the opacity of the sprite by
     * modifying the alpha channel during the render cycle. 1.0f is
     * opaque and 0.0f is completely transparent.
     *
     * @param [in] alpha The new value for the alpha channel.
    */
    void opacity(float alpha) noexcept;

    /**
     * @brief Retrieves the opacity of the sprite.
     *
     * This function let's you obtain the opacity of the sprite.
     * The opacity is controlled by using the alpha channel. 1.0f
     * is opaque and 0.0f is completely transparent.
     *
     * @return The value for the alpha channel.
     */
    [[nodiscard]] float opacity() const noexcept;

    /**
     * @brief Gets the source rectangle used for rendering.
     *
     * It is possible to render a subset of a sprite using a rectangle.
     * The rectangle will use [0] and [1] for the beginning x and y
     * position in the texture and [2] and [3] for the width and height.
     * This is useful when the texture is a sprite sheet.
     *
     * @return A pointer to the rectangle array used for the UV source.
     * @warning Make sure not to access the pointer out of bounds as this
     * will lead to undefined behaviour.
     */
    float* srcRect() noexcept;

    /**
     * @brief Gets a read-only source rectangle used for rendering.
     *
     * It is possible to render a subset of a sprite using a rectangle.
     * The rectangle will use [0] and [1] for the beginning x and y
     * position in the texture and [2] and [3] for the width and height.
     * This is useful when the texture is a sprite sheet.
     *
     * @return The rectangle (as a constant) used for the UV source.
     * @warning Make sure not to access the pointer out of bounds as this
     * will lead to undefined behaviour.
     */
    [[nodiscard]] const float* srcRect() const noexcept;

    /**
     * @brief Retrieves the sprite's 4 points in local space.
     *
     * Simply returns the sprite's original and unmodified dimensions.
     * No transforms will be applied, meaning rotation and scaling
     * are absent and the points can be considered as local or
     * model space. These points can be used as the starting basis of
     * an AABB or for additional collision detection.
     *
     * @return The four vertex bounds of the sprite.
     */
    [[nodiscard]] SpriteBounds getLocalBounds() const noexcept;

    /**
     * Retrieves the sprite's 4 points in world space.
     *
     * Returns the 4 vertices that delimit the sprite's position in
     * world space. Any transforms (scaling and rotation) have already
     * been applied. This is useful for when you need to know the
     * projected location i.e. calculating a bounding volume.
     *
     * @return The four vertex bounds of the sprite in world space.
     */
    [[nodiscard]] SpriteBounds getWorldBounds() const noexcept;

    /**
     * @brief Attaches a texture to the sprite.
     *
     * Sprites are simply textures which have transform and rendering
     * properties applied to them. They are mostly for convenience
     * of rendering. It is possible to swap out the texture attached
     * to the sprite. Doing so will result the UV coordinates and
     * width and height of the sprite to match the texture, so any
     * changes will need to be reapplied.
     *
     * @param texture2D The texture to attach.
     * @return True if successful.
     */
    virtual bool attach(ASGE::Texture2D* texture2D, AttachMode mode) noexcept = 0;

    virtual bool attach(ASGE::Texture2D* texture2D) noexcept;

    /**
     * @brief Calculates the mid-point of the sprite.
     *
     * Sprites origins are normally 0,0, which maps to the top left.
     * This function returns the mid-point of the sprite based on
     * its with and height and its current position in the world.
     *
     * @note This midpoint is scaled using the sprites scale factor.
     * @return The midpoint as an ASGE::Point2D
     * @see ASGE::Point2D
     */
    ASGE::Point2D midpoint() const;

    /**
     * @brief Retrieves the rendering order (layer) of the sprite.
     * @return The sprite's current z-order.
     */
    [[nodiscard]] int16_t getGlobalZOrder() const noexcept;

    /**
     * @brief Sets the rendering order (layer) of the sprite.
     *
     * When rendering it is often necessary to control the order in
     * which sprites, text or even UI elements are rendered. For example,
     * if parts of the UI were rendered before the background they would
     * end up hidden. Z-ordering is a common approach to controlling this
     * behaviour and is essential when using batch rendering.
     *
     * @param[in] new_z_order The z-order to apply to sprite.
     *
     * @note
     * Typically, an object with a lower z-order will be rendered before
     * one with a higher z-order. In ASGE you can use the z_order property
     * to control the render ordering.
     *
     * **Example**
     *
     * In this image you can see an example of when the z-order for
     * a sprite is not set correctly. The blocks below the player
     * should have a lower z-order than the player's sprite to ensure
     * they are rendered first.
     * @image html "https://i.stack.imgur.com/8aGig.png" "GameDev @ StackExchange" width=300
     */
    void setGlobalZOrder(int16_t new_z_order) noexcept;

    /**
     * @brief Retrieves any attached shader.
     * @return A pointer to the shader attached to the sprite. May be null.
     * @see ASGE::SHADER_LIB::Shader
     */
    [[nodiscard]] const SHADER_LIB::Shader* getPixelShader() const noexcept;

    /**
     * @brief Retrieves any attached shader.
     * @return A pointer to the shader attached to the sprite. May be null.
     * @see ASGE::SHADER_LIB::Shader
     */
    SHADER_LIB::Shader* getPixelShader() noexcept;

    /**
     * @brief Attaches a shader to the sprite.
     *
     * When rendering in ASGE, it is possible to create custom pixel
     * shaders that can have additional uniforms which can be updated
     * by the developer. You can either flag the renderer to switch
     * shaders, or if you only need a small subset of sprites to
     * use the custom shader, attach them directly. The renderer will
     * take care of switching shaders out when rendering to the GPU.
     *
     * @param[in] shader The shader to attach.
     */
    void setPixelShader(SHADER_LIB::Shader* shader) noexcept;

    /**
     * @brief Checks to see if this sprite has a pixel shader attached.
     * @return True if pixel shader is attached.
     */
    [[nodiscard]] bool hasPixelShader() const noexcept;

    /**
     * @brief Sets the magnification filter on the attached texture.
     *
     * Use this function to alter the texture sampling behaviour when the
     * attached texture is being read.
     *
     * @see ASGE::Texture2D::MagFilter
     * @param requested_filter
     */
    void setMagFilter(Texture2D::MagFilter requested_filter) const noexcept;

   protected:
    /**
     * @brief Retrieves the dimensions.
     * @return The array describing the width and height of the sprite.
     */
    std::array<float, 2>& dimensions();

   private:
    /**< Sprite Dimensions. The dimensions of the sprite. */
    std::array<float, 2> dims{ 0, 0 };
    /**< Sprite Position. The sprites position on the screen. */
    std::array<float, 2> position{ 0, 0 };
    /**< Sprite Rectangle. The source rectangle used for drawing. */
    std::array<float, 4> src_rect{ 0, 0, 0, 0 };

    int16_t z_order      = 0;    /**< Sprite Z-Order. Used to control the render layer.           */
    float angle          = 0.0F; /**< Sprite Rotation. Rotation around the sprite's origin.       */
    float scale_factor   = 1.0F; /**< Sprite Scale. Scales the sprite equally in both dims.       */
    float alpha          = 1.0F; /**< Sprite Opacity. Controls the sprite's opacity via alpha     */
    FlipFlags flip_flags = NORMAL; /**< Sprite Texture Flip. Flags to control UV mappings.        */
    Colour tint = COLOURS::WHITE; /**< Sprite Colour. Sets the colour of the sprite's vertices.   */
    SHADER_LIB::Shader* shader = nullptr; /**< Sprite Shader. Custom shader to render sprite with.*/
  };
} // namespace ASGE
