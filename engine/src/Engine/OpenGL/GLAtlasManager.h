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

#include "GLFontSet.hpp"
#include <Engine/Texture.hpp>
#include <deque>
#include <string>

#include "GLIncludes.hpp"
#include "GLRenderBatch.hpp"
#include "GLShader.hpp"

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
		int  loadFont(const char* font, int pt);
		int  loadFontFromMem(const char* name, const unsigned char* data, unsigned int size, int pt);

		[[nodiscard]] const GLFontSet& getFont(int idx) const;
		[[nodiscard]] const GLFontSet& getDefaultFont() const;

	 private:
		int  searchAtlas(const char * name, int pt);
		int  font = 0;
		std::deque<GLFontSet> font_sets;
    int createAtlas(FT_Face& face, const char* name, int pt);
    bool initFT();
  };
}  // namespace ASGE

