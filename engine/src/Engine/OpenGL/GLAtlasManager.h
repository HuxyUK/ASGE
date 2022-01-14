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

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <deque>
#include <string>
#include "GLFontSet.hpp"
#include "GLIncludes.hpp"
#include "GLRenderBatch.hpp"
#include "GLShader.hpp"
#include "Texture.hpp"

namespace ASGE
{
	class GLAtlasManager
  {
	public:
   GLAtlasManager() = default;
		~GLAtlasManager();
    GLAtlasManager(const GLAtlasManager&) = delete;
    GLAtlasManager operator=(const GLAtlasManager&) = delete;

		bool init();
		void setDefaultFont(int idx);
    const Font* loadFont(const char* font_path, int size, double range);
    const Font* loadFontFromMem(const char* name, const unsigned char* data, unsigned int len, int glyph_size, double range);
    const GLFontSet* loadFontFromAtlas(Font::AtlasMetrics&& metrics, std::string img_path, std::string csv_path);
    const GLFontSet* loadFontFromAtlas(Font::AtlasMetrics&& metrics, std::byte* data, std::string csv_data);
		[[nodiscard]] const GLFontSet& getFont(int idx) const;
		[[nodiscard]] const GLFontSet& getDefaultFont() const;

	 private:
    int searchAtlas(const char* name, int glyph_size);
    const Font* createAtlas(FT_Face& face, const char* name, int size, double range);
    const Font* build(int atlas_id, Font::AtlasMetrics& metrics, std::string& csv);
    bool initFT();
    int font = 0;
    std::deque<GLFontSet> font_sets;
  };
}  // namespace ASGE

