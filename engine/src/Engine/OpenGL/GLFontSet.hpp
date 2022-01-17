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
#include "Font.hpp"
#include <memory>
namespace ASGE
{
  // forward declarations
  class FontTextureAtlas;

  /**
   * An OpenGL implementation of an ASGE::Font
   *
   * Uses an atlas that is generated using texture sampling
   * and stores each characters properties i.e. UV, advance etc.
   * @see ASGE::Font
   */
  struct GLFontSet : public Font
  {
   public:
    GLFontSet() = default;
    GLFontSet(GLFontSet&&) noexcept;
    GLFontSet(const GLFontSet&) = delete;
    GLFontSet& operator=(const GLFontSet&) = delete;
    GLFontSet& operator=(GLFontSet&&) noexcept;
    ~GLFontSet() override;

    /**
     * @brief Retrieves the font atlas attached to the font.
     *
     * All fonts have an atlas attached to them. The atlas has the resultant
     * rendered glyphs stored in. When rendering characters, the UV coordinates
     * for the corresponding glyph is used as defined by its atlas.
     *
     * @return The attached font atlas.
     */
    [[nodiscard]] const FontTextureAtlas* getAtlas() const noexcept;

    /**
     * @brief Sets the atlas used by this font.
     *
     * The atlas is the rendered texture created when the font is loaded. It
     * stores a collection of glyphs along with their UV location used for
     * rendering.
     *
     * @param[in] atlas The atlas used for rendering this "font".
     */
    void setAtlas(FontTextureAtlas* atlas) noexcept;

    /**
     * @brief Concrete implementation of setMagFilter
     * Changes the magnification filter used when sampling the texture.
     * @param [in] mag_filter The magnification filter to use.
     */
    void setMagFilter(ASGE::Texture2D::MagFilter mag_filter) override;

    [[nodiscard]] float pxWide(const std::string& string, float scale) const override;
    [[nodiscard]] float pxWide(char ch, float scale) const noexcept;
    [[nodiscard]] float pxHeight(const std::string& string, float scale) const override;
    [[nodiscard]] std::tuple<float, float> boundsY(const std::string& string, float scale) const override;

   private:
    std::unique_ptr<FontTextureAtlas> atlas;
  };
} // namespace ASGE
