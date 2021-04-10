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
#include <string>
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

   public:
    const char* font_name = ""; //!< The name of the font loaded.
    int font_size = 0;          //!< The size of the font imported.
    int line_height = 0;        //!< The recommended height of each line.
  };
}  // namespace ASGE
