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

//! @file Font.hpp
//! @brief Struct
//! @ref ASGE::Font

#pragma once
#include "NonCopyable.hpp"
#include "Texture.hpp"
#include <string>
#include <tuple>
//#include "Align.hpp"
namespace ASGE
{
  /**
   * @brief A font used to render text
   *
   * A font is used to store font related information. All fonts need a name,
   * size and line height. This can be used by the renderer to ensure loaded
   * fonts are being used correctly.
   *
   * <example>
   *  Usage:
   *  @code
   *    // from binary data
   *    ASGE::FILEIO::File file;
   *    file.open("/data/Fonts/DroidSansMono.ttf", ASGE::FILEIO::File::IOMode::READ);
   *    ASGE::FILEIO::IOBuffer buffer = file.read();
   *    renderer->loadFontFromMem("DroidSansMono", buffer.as_unsigned_char(), static_cast<unsigned int>(buffer.length), 18);
   *
   *    // directly using a local file
   *    renderer->loadFont("/data/fonts/DroidSansMono.ttf", 18);
   *  @endcode
   * </example>
   *
   * @see Renderer
   * @see Text
   */
  //struct MS_ALIGN(16) Font : public NonCopyable
  struct Font : public NonCopyable
  {
   public:
    struct AtlasMetrics
    {
      std::string id     = "";
      int width          = 128;
      int height         = 128;
      double range       = 2;
      double size        = 32;
      double em_size     = 1;
      double line_height = 1;
      double ascender    = 1;
      double descender   = 1;
    };

    //! Default constructor
    Font() = default;

    //! Default destructor
    virtual ~Font() = default;

    /**
     * @brief Returns the distance in x pixels.
     *
     * Sometimes it's useful to know how wide a string rendered on the screen
     * will be, maybe for procedurally placing text etc. This function attempts
     * to return the number of pixels a string will take.
     *
     * @param[in] string the length you want to calculate.
     * @return The max number of pixels wide needed to render the string.
     */
    [[nodiscard]] int pxWide(const std::string& string) const;

    /**
     * @brief Returns the distance in x pixels.
     *
     * Sometimes it's useful to know how wide a string rendered on the screen
     * will be, maybe for procedurally placing text etc. This function attempts
     * to return the number of pixels a string will take.
     *
     * @param[in] ch The character to check.
     * @param[in] scale Any scaling to apply.
     * @return The max number of pixels wide needed to render the string.
     */
    [[nodiscard]] int pxWide(const char* ch, float scale) const;

    /**
     * @brief Returns the distance in y pixels.
     *
     * Sometimes it's useful to know how tall a string rendered on the screen
     * will be, maybe for procedurally placing text etc. This function attempts
     * to return the number of pixels the height of a string will take.
     *
     * @param[in] string the length you want to calculate.
     * @return The max number of pixels high needed to render the string.
     */
    [[nodiscard]] int pxHeight(const std::string& string) const;

    /**
     * @brief Returns the distance in y pixels.
     *
     * Sometimes it's useful to know how tall a string rendered on the screen
     * will be, maybe for procedurally placing text etc. This function attempts
     * to return the number of pixels the height of a string will take.
     *
     * @param[in] ch The height of the character to calculate.
     * @param[in] scale Any scaling to apply.
     * @return The max number of pixels high needed to render the string.
     */
    [[nodiscard]] int pxHeight(const char* ch, float scale) const;

    /**
     * @brief Returns the distance from the baseline in the y axis.
     *
     * Font's use a baseline for positioning the main body of text. This function
     * calculates the max distance on the Y axis required to render the text. It
     * returns both the deviation in the -Y and deviation in the +Y axis. This can
     * be used to create bounding boxes. It will also correctly parse new lines.
     *
     * @param[in] ch The character used in the calculations.
     * @param[in] scale Any scaling to apply.
     * @return The number of pixels required in the Y axis to render the string.
     */
    [[nodiscard]] std::tuple<float, float> boundsY(const char* ch, float scale) const;

    /**
     * @brief Returns the distance from the baseline in the y axis.
     *
     * Font's use a baseline for positioning the main body of text. This function
     * calculates the max distance on the Y axis required to render the text. It
     * returns both the deviation in the -Y and deviation in the +Y axis. This can
     * be used to create bounding boxes. It will also correctly parse new lines.
     *
     * @param[in] str The string used in the calculations.
     * @param[in] scale Any scaling to apply.
     * @return The number of pixels required in the Y axis to render the string.
     */
    [[nodiscard]] virtual std::tuple<float, float>
    boundsY(const std::string& string, float scale) const = 0;

    /**
     * @brief Returns the distance scaled in x pixels.
     *
     * Sometimes it's useful to know how wide a string  placing text etc. This
     * function attempts to return rendered on the screen will be, maybe for
     * procedurally placing text etc. This function attempts to return
     * the number of pixels a string will take.
     *
     * @param[in] string The length you want to calculate.
     * @param[in] scale The scaled size you want to calculate.
     * @return The max number of pixels wide needed to render the string.
     */
    [[nodiscard]] virtual float
    pxWide(const std::string& string, float scale) const = 0;

    /**
     * @brief Returns the distance scaled in y pixels.
     *
     * Sometimes it's useful to know how wide a string rendered on the screen
     * will be, maybe for procedurally placing text etc. This function attempts
     * to return the number of pixels a string will take vertically.
     *
     * @param[in] string The length you want to calculate.
     * @param[in] scale the scaled size you want to calculate.
     * @return The max number of pixels wide needed to render the string.
     */
    [[nodiscard]] virtual float
    pxHeight(const std::string& string, float scale) const = 0;

    /**
     * Sets the filtering used for scaling the font upwards.
     * The mag filter controls how a texture's sampling will operate
     * when magnified. In general use, font's work well with linear
     * sampling, but this allows the user to specify nearest neighbour
     * instead which may lead to crisper results depending on the font
     * face.
     * @param[in] mag_filter The magnification filter to use.
     */
    virtual void setMagFilter(ASGE::Texture2D::MagFilter mag_filter) = 0;

   public:
    const char* font_name = ""; //!< The name of the font loaded.
    float line_height     = 0;  //!< The recommended height of each line.
    float px_range        = 2;  //!< The pixel range used in the SDF generation.
    int  font_size        = 0;  //!< The size of the font imported.
  };
}  // namespace ASGE
