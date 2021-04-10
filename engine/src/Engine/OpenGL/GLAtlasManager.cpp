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

#include "GLAtlasManager.h"
#include "GLAtlas.hpp"
#include "GLFontSet.hpp"
#include <Engine/FileIO.hpp>

#include "Fonts/Hermit.font"
#include "Shaders/GLShaders.fs"
#include "Shaders/GLShaders.vs"

namespace
{
  FT_Library ft;
}

bool ASGE::GLAtlasManager::init()
{
  if (!initFT())
  {
    return false;
  }

  return loadFontFromMem("default", Hermit_medium_otf, sizeof(Hermit_medium_otf), 18) != -1;
}

bool ASGE::GLAtlasManager::initFT()
{
  if (FT_Init_FreeType(&ft))
  {
    Logging::ERRORS("FREETYPE: library could not be initialised");
    return false;
  }

  return true;
}

int ASGE::GLAtlasManager::searchAtlas(const char* name, int pt)
{
	auto iter = std::find_if(
	  font_sets.begin(), font_sets.end(),
	  [name, pt](const GLFontSet& fs)
	{
    return fs.font_name == name &&
           fs.font_size == pt &&
           (fs.getAtlas() != nullptr);
  });

	if (iter != font_sets.end())
	{
		return static_cast<int>((iter - font_sets.begin()));
	}

	return -1;
}

int ASGE::GLAtlasManager::loadFontFromMem(
  const char* name, const unsigned char* data, unsigned int size, int pt)
{
	// Let's check to see if a valid atlas is already loaded for this font
	int fontid = searchAtlas(name, pt);
	if (fontid != -1) {
		return fontid;
  }

	// Load font as face
	FT_Face face = nullptr;
	if (FT_New_Memory_Face(ft, data, size, 0, &face) != 0)
	{
    Logging::ERRORS("FREETYPE: font could not be loaded from memory");
		return -1;
	}

  return createAtlas(face, name, pt);
}

int ASGE::GLAtlasManager::loadFont(const char* font_path, int pt)
{
  // Let's check to see if a valid atlas is already loaded for this font
  int fontid = searchAtlas(font_path, pt);
  if (fontid != -1) {
    return fontid;
  }

  // Attempt to use ASGE File IO
  ASGE::FILEIO::File file;
  if( file.open(font_path, ASGE::FILEIO::File::IOMode::READ) )
  {
    ASGE::FILEIO::IOBuffer buffer = file.read();
    return loadFontFromMem(
      font_path, buffer.as_unsigned_char(), static_cast<unsigned int>(buffer.length), pt);
  }

  // Load font as face
  FT_Face face = nullptr;
  if (FT_New_Face(ft, font_path, 0, &face) != 0)
  {
    Logging::ERRORS("FREETYPE: could not load " + std::string(font_path));
    return -1;
  }

  return createAtlas(face, font_path, pt);
}

int ASGE::GLAtlasManager::createAtlas(FT_Face& face, const char* name, int pt)
{
  GLFontSet set;
  set.font_name = name;
  set.font_size = pt;

  auto *atlas = new FontTextureAtlas();
  if (atlas->init(face, pt))
  {
    set.setAtlas(atlas);
  }
  else
  {
    Logging::ERRORS( "Renderer failed to initialise the font atlas for: ");
    Logging::ERRORS( "\t " + std::string(name));

    delete atlas;
    return -1;
  }

  // store the line spacing
  set.line_height =
    static_cast<int>(
      (face->size->metrics.ascender - face->size->metrics.descender)) / 64;

  FT_Done_Face(face);

  font_sets.push_back(std::move(set));
  return static_cast<int>(font_sets.size() - 1);
}

const ASGE::GLFontSet& ASGE::GLAtlasManager::getFont(int idx) const
{
  if(uint64_t(idx) < font_sets.size())
  {
    return font_sets[idx];
  }

return getDefaultFont();
}

const ASGE::GLFontSet& ASGE::GLAtlasManager::getDefaultFont() const
{
	return getFont(font);
}

/**
*  Sets the current font that will be used for rendering
*  text to the screen. It uses the index of a pre-loaded
*  font. They can be loaded via file or from a buffer
*  in memory.
*  param [in] idx The index of the pre-loaded font to use
*/
void ASGE::GLAtlasManager::setDefaultFont(int idx)
{
  font = idx;
}

ASGE::GLAtlasManager::~GLAtlasManager()
{
  FT_Done_FreeType(ft);
}
