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

//! @file Renderer.hpp
//! @brief Class @ref ASGE::Renderer

#pragma once
#include "Camera.hpp"
#include "Engine/Colours.hpp"
#include "Engine/Texture.hpp"
#include "GameSettings.hpp"
#include "Text.hpp"
#include "Viewport.hpp"
#include <memory>
#include <string>

namespace ASGE {

	namespace SHADER_LIB
	{
		class  Shader;
	}

	struct Font;
	class  Input;
  class  RenderTarget;
  class  Sprite;

  /**
   *  @brief The renderer used in the game.
   *
   *  The renderer is the heart and sole of the engine. It's
   *  responsible for managing rendering, shaders input
   *  initialisation, the window etc. This class defines the interface
   *  that all platform specific renders must utilise. This allows
   *  renderers to be replaced easily if their interface matches.
   *  The platform specific renders may use batching or texture
   *  caching but this is not enforced.
   */
  class Renderer
	{

	public:

    /**
     *  @brief Default Constructor.
     *
     *  The renderer is an abstract class that does not know about the
     *  rendering implementation details. Its primary use is as an interface
     *  that all rendering subsystems need to adhere to.
     */
    Renderer() = default;

   /**
    *  Default destructor.
    */
   virtual ~Renderer() = default;

    /**
     *  @brief Sets the clear colour.
     *
     *  This colour will be used to clear the window on every update. Allowing
     *  a clean slate before performing the next render.
     *
     *  @param rgb[in] A RGB defined colour to use for clearing.
     *  @see Colour
     */
    virtual void setClearColour(Colour rgb) = 0;

   /**
    *  Loads a font that can be used to render text
    *
    *  @param[in] font The filepath to the font file.
    *  @param[in] pt The size of the font to use in atlas generation.
    *
    *  @return The loaded font index.
    */
   virtual int loadFont(const char* font, int pt) = 0;

    /**
     *  Loads a font that can be used to render text
     *
     *  @param[in] name The name of the font.
     *  @param[in] data A binary font loaded in memory.
     *  @param[in] size The buffer size.
     *  @param[in] pt The size of the font to use in atlas generation.
     *
     *  @return The loaded font index.
     */
    virtual int loadFontFromMem(const char* name, const unsigned char* data, unsigned int size, int pt) = 0;

   /**
    *  @brief Initialises the renderer.
    *
    *  Performs all the initialisation of the renderer, its framework and the
    *  window creation.
    *
    *  @param[in] w The width of the window in pixels.
    *  @param[in] h The height of the window in pixels.
    *  @param[in] mode Should the window launch in fullscreen.
    *
    *  @return True if success.
    *  @see WindowMode
    */
   virtual bool init() = 0;

		/**
		*  Exits the renderer and performs clean-up. 
		*  @return True if achieved gracefully.
		*/
		virtual bool exit() = 0;

   /**
    *  @brief Sets the pre render environment.
    *
    *  Can be used amongst other things to reset render settings and to
    *  clear the screen.
    */
   virtual void preRender() = 0;

    /**
     *  @brief Sets the post render environment.
     *
     *  Can be used amongst other things to update OSD settings and to flush
     *  batched render calls.
     */
    virtual void postRender() = 0;

   /**
    *  Renders a string to the screen.
    *  @deprecated since 2.0
    *  @see RenderString
    *
    *  @param [in] str The text to render.
    *  @param [in] x The text position in the X axis.
    *  @param [in] y The text starting position in the Y axis.
    *  @param [in] colour The colour to use for rendering.
    */
   void renderText(std::string str, int x, int y, const Colour& colour);

    /**
     *  Renders a string to the screen.
     *  @deprecated since 2.0
     *  @see RenderString
     *
     *  @param [in] str The text to render.
     *  @param [in] x The text position in the X axis.
     *  @param [in] y The text starting position in the Y axis.
     */
    void renderText(std::string str, int x, int y);

    /**
     * Renders a text object.
     * @param[in] text The temporary text object to render.
     */
    virtual void renderText(Text&& text) = 0;

    /**
     * Renders a text object.
     * @param[in] text The text object to render.
     */
    virtual void renderText(const Text& text) = 0;

    /**
     *  @brief Sets the default text colour.
     *
     *  When rendering text, if a colour is not specified the default one will
     *  be used instead. This can be set using this function.
     *
     *  @param[in] colour The colour to use.
     */
    virtual void setDefaultTextColour(const Colour& colour) = 0;

    /**
     *  @brief Locates a shader using its ID.
     *
     *  Shaders are stored/cached so to prevent dangling pointers
     *  handle's can be used to retrieve the shader. It is recommended
     *  not to store the returned shader as it's memory location could
     *  change over time.
     *
     *  @param[in] shader_handle The shader to find.
     */
    virtual SHADER_LIB::Shader* findShader(int shader_handle) = 0;

    /**
     *  @brief Returns the currently active font.
     *
     *  When rendering text, if a colour is not specified
     *  the default one will be used instead. This can be set
     *  using this function.
     *
     *  @return Font The font currently being used by the renderer.
     *  @see Font
     */
    [[nodiscard]] virtual const Font& getDefaultFont() const = 0;

    /**
     *  @brief Returns a font.
     *
     *  When loading fonts they will be assigned an index, you can
     *  retrieve the font using its index to access additional
     *  font related functionality
     *
     *  @param[in] idx The loaded font index
     *  @return Font The font stored in idx otherwise current loaded
     *  @see Font
     */
    [[nodiscard]] virtual const Font& getFont(int idx) const = 0;

    /**
     *  @brief Sets the currently active font.
     *
     *  In order for this to work, a valid id must be provided.
     *  The ID is returned from the load font function, so it is
     *  advisable to keep a record of it.
     *
     *  @param[in] id The index of the font to use (must be loaded).
     *  @deprecated since 2.1
     */
    virtual void setFont(int id) = 0;

    /**
     *  @brief Renders a sprite to the screen.
     *
     *  All the rendering params are stored within the sprite
     *  class itself.
     *
     *  @param [in] sprite A reference to the sprite to render.
     *  @see Sprite
     *  @deprecated since 2.1
     */
    virtual void renderSprite(const Sprite& sprite) = 0;

    /**
     *  Sets the sprite rendering mode. Useful for batching.
     *  @param[in] mode The SpriteSortMode used for rendering.
     *  @see SpriteSortMode
     */
    virtual void setSpriteMode(SpriteSortMode mode) = 0;

    /**
     *  @brief Attempts to enable the requested window mode.
     *
     *  Will only swap if the mode is different and will attempt
     *  to perform this operation gracefully as possible.
     *
     *  @param[in] mode The requested window mode.
     */
    virtual void setWindowedMode(GameSettings::WindowMode mode) = 0;

    /**
     *  Sets the window title.
     *  @param[in] str The name of the window.
     */
    virtual void setWindowTitle(const char* str) = 0;

    /**
     *  Tells the graphics subsystem to swap buffers.
     */
    virtual void swapBuffers() = 0;

    /**
     *  Creates an input system, linked to the renderer.
     *  @return A uniquely owned input system.
     */
    virtual std::unique_ptr<Input> inputPtr() = 0;

    /**
     *  @brief Creates a new Sprite using ownership semantics.
     *
     *  The sprite will be auto-deallocated when it falls out of
     *  scope, so be careful where to store it.
     *
     *  @return A uniquely owned sprite.
     */
    virtual std::unique_ptr<Sprite> createUniqueSprite() = 0;

    /**
     *  @brief Creates a new Sprite using the heap.
     *
     *  The sprite will be leaked when it falls out of scope, so
     *  needs to be manually freed.
     *
     *  @return A dynamically allocated sprite.
     */
    virtual Sprite*	createRawSprite() = 0;

    /**
     * Renders a sprite.
     * @param[in] sprite The sprite to render.
     */
    virtual void render(const ASGE::Sprite& sprite);

    /**
     * Renders a text object.
     * @param[in] text The text object to render.
     */
    virtual void render(const ASGE::Text& text);

    /**
     * Renders a temporary text object.
     * @param[in] text The temporary text object to render.
     */
    virtual void render(const ASGE::Text&& text);

    /**
     * Renders a texture.
     *
     * @param[in] texture The texture to render.
     * @param[in] x The x position of the rendered texture.
     * @param[in] y The y position of the rendered texture.
     */
    virtual void render(ASGE::Texture2D& texture, int x, int y) = 0;

    /**
     * @brief Creates a non-cached texture.
     *
     * A non-cached texture is created and assigned to the GPU.
     * The size of the allocation depends on both the dimensions and
     * the pixel format. As the texture is not cached by ASGE,
     * responsibility for its destruction is left to the callee.
     *
     * @param[in] width The width of the newly created texture.
     * @param[in] height The height of the newly created texture.
     * @param[in] format The pixel format.
     * @param[in] data Existing pixel data to use. Can be nullptr (empty).
     *
     * @return The newly created Texture.
     * @warning Don't forget to deallocate the memory the function call allocates.
     */
    virtual ASGE::Texture2D* createNonCachedTexture(int width, int height, ASGE::Texture2D::Format format, void* data) = 0;

    /**
     * @brief Attempts to create a non-cached texture from a file.
     *
     * A non-cached texture is created and assigned to the GPU.
     * The size of the allocation depends on both the dimensions and
     * the pixel format. As the texture is not cached by ASGE,
     * responsibility for its destruction is left to the callee.
     *
     * @param[in] path The texture to load.
     *
     * @return The newly created Texture.
     * @warning Don't forget to deallocate the memory the function call allocates.
     */
    virtual ASGE::Texture2D* createNonCachedTexture(const std::string& path) = 0;

    /**
     * @brief Creates a cached texture.
     *
     * A cached texture is one which is managed by the renderer.
     * Upon its successful creation it will be stored using its ID.
     * Subsequent retrievals of a texture with the same ID will
     * result in the cached version being returned.
     *
     * @param[in] id The unique ID for the texture.
     * @param[in] width The width of the newly created texture.
     * @param[in] height The height of the newly created texture.
     * @param[in] format The pixel format.
     * @param[in] data Existing pixel data to use. Can be nullptr (empty).
     *
     * @return A pointer to the cached @refitem ASGE::Texture2D matching the
     * requested ID
     */
    virtual ASGE::Texture2D* createCachedTexture(std::string id, int width, int height, ASGE::Texture2D::Format format, void* data) = 0;

    /**
     * @brief Creates a cached texture from a file.
     *
     * A cached texture is one which is managed by the renderer.
     * Upon its successful creation it will be stored using its
     * file path. Subsequent retrievals of a texture with the same
     * ID will result in the cached version being returned.
     *
     * @param[in] path The texture to load.
     * @return The newly cached Texture.
     */
    virtual ASGE::Texture2D* createCachedTexture(const std::string& path) = 0;

    /**
     * Initialises a pixel shader stored from a local file.
     *
     * @param[in] filename The file to load.
     * @return The resultant shader object.
     *
     * @note The shader is managed by the renderer. It does not need to be
     * manually deallocated.
     */
    SHADER_LIB::Shader* initPixelShaderFromFile(const std::string& filename);

    /**
     * Initialises a pixel shader.
     *
     * <example>
     * **GLSL Example**
     * @code
     *   #version 330 core
     *   #define FRAG_COLOUR     0
     *   in VertexData
     *   {
     *     vec2    uvs;
     *     vec4    rgba;
     *   } fs_in;
     *
     *   uniform sampler2D image;
     *   layout  (location = FRAG_COLOUR, index = 0) out vec4 FragColor;
     *
     *   void main()
     *   {
     *     FragColor = fs_in.rgba * texture(image, fs_in.uvs);
     *   }
     * @endcode
     * </example>
     *
     * @param[in] shader The plain text representing a pixel shader.
     * @return The resultant shader object.
     * @note The shader is managed by the renderer. It does not need to be
     * manually deallocated.
     */
    virtual SHADER_LIB::Shader* initPixelShader(std::string shader) = 0;

    /**
     * Sets the shader to use when rendering.
     * @param[in] shader The shader to activate.
     */
		virtual void setActiveShader(ASGE::SHADER_LIB::Shader* shader) = 0;

    /**
     * Retrieves the current shader being used for rendering.
     * @return Pointer to the active shader (if any).
     */
    virtual SHADER_LIB::Shader* getShader() = 0;

    /**
     * Retrieves the window mode in use.
     * @return The window mode.
     */
    [[nodiscard]] GameSettings::WindowMode getWindowMode() noexcept;

    /**
     * Retrieves the window height.
     * @return The window height.
     */
    [[nodiscard]] virtual float windowHeight() const noexcept = 0;

    /**
     * Retrieves the window width.
     * @return The window width.
     */
    [[nodiscard]] virtual float windowWidth()  const noexcept = 0;

    /**
     * @brief Sets the projection matrix
     *
     * Controls how the scene is rendered to the window. The
     * projection matrix can be thought of as the camera's view,
     * detailing the focal point and the content to include either
     * side of the mid-point. This projection matrix may be stretched
     * to accommodate the viewport used to render to the window.
     *
     * @param[in] min_x The camera's look at x position.
     * @param[in] max_x The camera's look at y position.
     * @param[in] min_y The camera's frustum width.
     * @param[in] max_y The camera's frustum height.
     */
    virtual void setProjectionMatrix(float min_x, float max_x, float min_y, float max_y) = 0;

    /**
     * @brief Sets the projection matrix
     *
     * Controls how the scene is rendered to the window. The
     * projection matrix can be thought of as the camera's view,
     * detailing the focal point and the content to include either
     * side of the mid-point. This projection matrix may be stretched
     * to accommodate the viewport used to render to the window.
     *
     * @param[in] view. The camera view to use.
     */
    virtual void setProjectionMatrix(const Camera::CameraView& view) = 0;

    /**
     * @brief Gets the viewport used to map the renderer to the window.
     * @return Retrieves the active viewport.
     */
    [[nodiscard]] virtual ASGE::Viewport getViewport() const = 0;

    /**
     * @brief Sets the viewport used to map the renderer to the window.
     * @param[in] viewport Sets the viewport.
     */
    virtual void setViewport(const ASGE::Viewport& viewport) = 0;

    /**
    *  @brief Sets the render target to use for rendering.
     *
    *  Instead of rendering directly to the screen, a render target with
    *  an attached texture can be directly rendered to. This allows for
    *  off-screen rendering as well as an easier to manage post-fx pipeline.
     *
    *  @param[in] render_target. The destination target.
    */
    virtual void setRenderTarget(const ASGE::RenderTarget*) = 0;


	private:
		GameSettings::WindowMode window_mode = GameSettings::WindowMode::WINDOWED; /**< The window mode being used. */
		Colour cls = COLOURS::STEELBLUE; /**< The clear colour. Used to blank the window every redraw. */
		Colour default_text_colour = COLOURS::YELLOWGREEN; /**< The default text colour. Used when no colour is specified. */

   protected:
    [[nodiscard]] GameSettings::WindowMode& windowMode() { return window_mode; }
    [[nodiscard]] const GameSettings::WindowMode& windowMode() const { return window_mode; }
    [[nodiscard]] Colour& clearColour() { return cls; }
    [[nodiscard]] const Colour& clearColour() const { return cls; }
    [[nodiscard]] Colour& defTextColour() { return default_text_colour; }
    [[nodiscard]] const Colour& defTextColour() const { return default_text_colour; }
	};
}  // namespace ASGE
