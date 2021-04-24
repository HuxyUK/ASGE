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

//! @file Text.hpp
//! @brief Struct @ref ASGE::Text

#ifndef ASGE_TEXT_HPP
#define ASGE_TEXT_HPP

#include "Colours.hpp"
#include "Font.hpp"
#include "SpriteBounds.hpp"
#include <string>

namespace ASGE
{
  /**
   * @brief Text is designed to allow rendering of text to the screen.
   *
   * Rendering text to a screen is not a simple process. There are lots of
   * parameters involved, which can result in many different overloaded
   * render functions. To resolve this, the ASGE::Text was created. All the
   * information needed to successfully render text to the screen is stored
   * within the struct. This includes positioning, colours, scaling, the text
   * and even the font face to use.
   *
   * Advantages of using this class allow a single instance to be constructed
   * and used repeatedly as well as the ability to add additional features to
   * the Text class without having to modify the renderer API.
   *
   * **Positioning**
   *
   * There are always a lot of questions around positioning of text on the
   * screen. The issue comes from the x and y actually controlling the position
   * of the baseline. The baseline is the point at which all characters should
   * lay along and allows consistent positioning of text irrespective of the
   * strings context.
   *
   * @image html "https://www.freetype.org/freetype2/docs/glyphs/layout.png" "The FreeType project" width=50%
   *
   * **Usage**
   * @code
   * // implicit construction
   * renderer->renderText({font_face, "hello world", 50, 50});
   *
   * // explicit construction
   * ASGE::Text text{font_face};
   * text.setString("Hello World. How are you today?")
   *     .setColour(ASGE::COLOURS::RED)
   *     .setPosition({ 100, 100 });
   * @endcode
   *
   * @warning
   * In order for text to be rendered and for any width and height calculations
   * to be performed a font face is required. If a font face is not provided the
   * width and height functions will return 0 and the renderer will fallback on
   * the engine's debug font for rendering.
   */
  struct Text
  {
   public:
    Text() = default;
    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;
    Text(Text&&) noexcept;
    Text& operator=(Text&& other) noexcept;
    ~Text() = default;

    /**
     * Constructor that takes a loaded font face
     * @param[in] font_face The font face to use for rendering.
     */
    explicit Text(const ASGE::Font& font_face);

    /**
     * Constructs a simple text object capable of rendering a string.
     * @param[in] font_face The font face to use.
     * @param[in] text The text to render or perform calculations on.
     */
    Text(const ASGE::Font& font_face, std::string  text);

    /**
     * Constructs a simple text object and positions it.
     * @param[in] font_face The font face to use.
     * @param[in] text The text to render or perform calculations on.
     * @param[in] x The x starting position of the baseline.
     * @param[in] y The y position of the baseline.
     */
    Text(const ASGE::Font& font_face, const std::string& text, int x, int y);

    /**
     * Constructs a simple text object and positions it.
     * @param[in] font_face The font face to use.
     * @param[in] text A temporary text object to store as the font's contents.
     * @param[in] x The x starting position of the baseline.
     * @param[in] y The y position of the baseline.
     */
    Text(const ASGE::Font& font_face, std::string&& text, int x, int y);

    /**
     * Constructs a simple text object, set's it's colour and positions it.
     * @param[in] font_face The font face to use.
     * @param[in] text The text to render or perform calculations on.
     * @param[in] x The x starting position of the baseline.
     * @param[in] y The y position of the baseline.
     * @param[in] colour The colour to render the text.
     */
    Text(const ASGE::Font& font_face, const std::string& text, int x, int y, const ASGE::Colour& colour);

    /**
     * Constructs a simple text object and positions it.
     * @param[in] font_face The font face to use.
     * @param[in] text A temporary text object to store as the font's contents.
     * @param[in] x The x starting position of the baseline.
     * @param[in] y The y position of the baseline.
     * @param[in] colour The colour to render the text.
     */
    Text(const ASGE::Font& font_face, std::string&& text, int x, int y, const ASGE::Colour& colour);


    /**
     * Checks to see if a valid font has been assigned.
     * @return Whether or not a font is attached.
     */
    [[nodiscard]] bool validFont() const noexcept;

    /**
     * @brief The z-order to control rendering.
     *
     * Like any other renderable object, the z-order can be use to manipulate
     * the rendering order. Text with lower values will be hidden by sprites
     * with higher values.
     *
     * @return Returns the text objects current z order.
     */
    [[nodiscard]] int16_t getZOrder() const noexcept;

    /**
     * Attempts to calculate the spacing between lines.
     * @return The line spacing between adjacent lines.
     */
    [[nodiscard]] int getLineSpacing() const;

    /**
     * @brief Calculates the max height of the stored text.
     *
     * This is obviously font face dependent and also dependent on the scale
     * factor applied to the font object. This can be used to calculate an
     * offset in the Y when positioning the text on the screen.
     *
     * @return The height.
     */
    [[nodiscard]] float getHeight() const noexcept;

    /**
     * The opacity of the font.
     * @return The alpha value used when rendering the font.
     */
    [[nodiscard]] float getOpacity() const noexcept;

    /**
     * The scale to apply to the text.
     * @return The scale to be applied when rendering.
     */
    [[nodiscard]] float getScale() const noexcept;

    /**
     * @brief Calculates the width of the rendered string.
     *
     * This is obviously font face dependent and also dependent on the scale
     * factor applied to the font object. This can be used to calculate an
     * offset in the x when positioning the text on the screen.
     *
     * @return The width.
     */
    [[nodiscard]] float getWidth() const noexcept;

    /**
     * The position of the text's baseline.
     * @return The starting point used for the baseline when rendering.
     * @see ASGE::Point2D
     */
    [[nodiscard]] const Point2D& getPosition() const noexcept;

    /**
     * Retrieves the colour of the font.
     * @return The ASGE::Colour value to use when rendering.
     */
    [[nodiscard]] const Colour& getColour() const noexcept;

    /**
     * Retrieves the string to be rendered.
     * @return The string being managed by this text.
     */
    [[nodiscard]] const std::string& getString() const noexcept;

    /**
     * Retrieves the font face currently assigned to the text object.
     * @return The font being used.
     * @see ASGE::Font
     */
    [[nodiscard]] const Font& getFont() const noexcept;

    /**
     * @brief Returns the bounds of the rendered text in world space.
     *
     * Calculates the width and height of the text in local space and then
     * positions it within a bounding box that retains it's position in world
     * space.
     *
     * @return The world bounds.
     */
    [[nodiscard]] TextBounds getWorldBounds() const;

    /**
     * @brief Returns the bounds of the rendered text in local space.
     *
     * Whilst this does not provide the absolute positioning of the text in the
     * game world, it does allow an easy way to calculate the bounding box
     * dimensions of the rendered text i.e. how wide and tall it is.
     *
     * @return The local bounds.
     */
    [[nodiscard]] TextBounds getLocalBounds() const;

    /**
     * Sets the Z order of the rendered output.
     * @param[in] z_order The new z-order to apply.
     * @return The text instance.
     */
    Text& setZOrder(int16_t z_order) noexcept;

    /**
     * Sets the scale factor of the rendered output.
     * @param[in] scale The scaling to apply.
     * @return The text instance.
     */
    Text& setScale(float scale) noexcept;

    /**
     * Sets the opacity of the rendered output (alpha channel).
     * @param[in] opacity The alpha channel level.
     * @return The text instance.
     */
    Text& setOpacity(float opacity) noexcept;

    /**
     * Updates the baseline x starting position.
     * @param[in] baseline_x The starting position of the baseline on the X axis.
     * @return The text instance.
     */
    Text& setPositionX(float baseline_x) noexcept;

    /**
     * Updates the baseline y starting position.
     * @param[in] baseline_yThe starting position of the baseline on the Y axis.
     * @return The text instance.
     */
    Text& setPositionY(float baseline_y) noexcept;

    /**
     * Updates the baseline position used to control the text's location
     * @param[in] baseline The starting position for the text.
     * @return The text instance.
     * @see ASGE::Point2D
     */
    Text& setPosition(const Point2D& baseline) noexcept;

    /**
     * Updates the baseline position used to control the text's location
     * @param[in] baseline The starting position for the text.
     * @return The text instance.
     * @see ASGE::Point2D
     */
    Text& setPosition(Point2D&& baseline) noexcept;

    /**
     * Sets the colour to render the text in.
     * @param[in] colour The colour to render the text.
     * @return The text instance.
     * @see ASGE::Colour
     */
    Text& setColour(const ASGE::Colour& colour) noexcept;

    /**
     * Replaces the text to be rendered using this object.
     * @param[in] string The replacement text.
     * @return The text instance.
     */
    Text& setString(const std::string& string) noexcept;

    /**
     * Replaces the text to be rendered using this object.
     * @param[in] string The replacement text.
     * @return The text instance.
     */
    Text& setString(std::string&& string) noexcept;

    /**
     * Replaces the font face.
     * @param[in] font_face The font face to use for this text object.
     * @return The text instance.
     */
    Text& setFont(const Font& font_face) noexcept;

   private:
    Colour colour      = ASGE::COLOURS::WHITE;
    Point2D position   = Point2D{0, 0};
    std::string string = "";
    const Font* font   = nullptr;
    float opacity      = 1.0F;
    float scale        = 1.0F;
    short z_order      = 0;

    int getCharacterSize (){static_assert(true, "Not Yet Implemented"); return 0;}
    int getRotation()      {static_assert(true, "Not Yet Implemented"); return 0;}
    int getLetterSpacing() {static_assert(true, "Not Yet Implemented"); return 0;}

  };
}  // namespace ASGE
#endif // ASGE_TEXT_HPP
