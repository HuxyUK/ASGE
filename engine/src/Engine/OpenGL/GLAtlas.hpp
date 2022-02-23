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
#include "GLIncludes.hpp"
#include <map>

typedef struct FT_FaceRec_*  FT_Face;
namespace msdfgen
{
  class FontHandle;
}

namespace ASGE
{
	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character
	{
    glm::dvec4 UV;      // UV coordinates of glyph
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
		glm::ivec2 Advance; // Offset to the next glyph
	};

	class FontTextureAtlas
	{
	public:
    friend class GLAtlasManager;
		FontTextureAtlas() = default;
		~FontTextureAtlas();
		FontTextureAtlas(const FontTextureAtlas&) = delete;
		FontTextureAtlas operator=(const FontTextureAtlas&) = delete;

    bool init(const FT_Face& face, msdfgen::FontHandle* font_handle, double glyph_size, double range, double font_scale);
		[[nodiscard]] GLuint getTextureID() const noexcept;
		[[nodiscard]] const Character& getCharacter(int idx) const;

   private:
    void allocateTexture(const void* data);
    void setSampleParams();

    std::map<int, Character> characters;
    GLuint texture = 0;
    int32_t width  = 0;
    int32_t height = 0;
  };
}  // namespace ASGE