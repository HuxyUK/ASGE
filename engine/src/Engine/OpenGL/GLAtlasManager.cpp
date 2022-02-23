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


#include <stb_image.h>

#include "FileIO.hpp"
#include "Fonts/ken_csv.hpp"
#include "Fonts/ken_png.hpp"
#include "GLAtlas.hpp"
#include "GLAtlasManager.h"
#include "GLFontSet.hpp"
#include "Logger.hpp"
#include "Shaders/GLShaders.fs"
#include "Shaders/GLShaders.vs"
#include "GLTexture.hpp"
#include "GLTextureCache.hpp"

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <filesystem>

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

  ASGE::Font::AtlasMetrics metrics;
  metrics.id = "default";
  metrics.width = 408;
  metrics.height = 408;
  metrics.range = 4.0;
  metrics.em_size = 1.0;
  metrics.line_height = 1.125;
  metrics.ascender = -0.875;
  metrics.descender = 0.25;
  metrics.size = 64;
  std::string csv { &kv_csv[0], &kv_csv[0] + kv_csv_len };
  auto * default_font = loadFontFromAtlas(std::move(metrics), reinterpret_cast<std::byte*>(kenvector_future_png), csv);
  return default_font != nullptr;
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

int ASGE::GLAtlasManager::searchAtlas(const char* name, int glyph_size)
{
	auto iter = std::find_if(
	  font_sets.begin(), font_sets.end(),
	  [name, glyph_size](const GLFontSet& fs)
	{
    return fs.font_name == name &&
           fs.font_size == glyph_size &&
           (fs.getAtlas() != nullptr);
  });

	if (iter != font_sets.end())
	{
		return static_cast<int>((iter - font_sets.begin()));
	}

	return -1;
}

const ASGE::Font* ASGE::GLAtlasManager::loadFontFromMem(
  const char* name, const unsigned char* data, unsigned int len, int glyph_size, double range)
{
	// Let's check to see if a valid atlas is already loaded for this font
	int idx = searchAtlas(name, glyph_size);
	if (idx != -1) {
		return &font_sets[idx];
  }

	// Load font as face
	FT_Face face = nullptr;
	if (FT_New_Memory_Face(ft, data, len, 0, &face) != 0)
	{
    Logging::ERRORS("FREETYPE: font could not be loaded from memory");
		return nullptr;
	}

  return createAtlas(face, name, glyph_size, range);
}

const ASGE::Font* ASGE::GLAtlasManager::loadFont(const char* font_path, int size, double range)
{
  // Let's check to see if a valid atlas is already loaded for this font
  int idx = searchAtlas(font_path, size);
  if (idx != -1) {
    return &font_sets[idx];
  }

  // Attempt to use ASGE File IO
  ASGE::FILEIO::File file;
  if( file.open(font_path, ASGE::FILEIO::File::IOMode::READ) )
  {
    ASGE::FILEIO::IOBuffer buffer = file.read();
    return loadFontFromMem(
      font_path,
      buffer.as_unsigned_char(),
      static_cast<unsigned int>(buffer.length), size, range);
  }

  // Load font as face
  FT_Face face = nullptr;
  if (FT_New_Face(ft, font_path, 0, &face) != 0)
  {
    Logging::ERRORS("FREETYPE: could not load " + std::string(font_path));
    return nullptr;
  }

  return createAtlas(face, font_path, size, range);
}

const ASGE::Font*
ASGE::GLAtlasManager::createAtlas(FT_Face& face, const char* name, int size, double range)
{
  Logging::TRACE("atlas time started: " + std::string(face->family_name));

  GLFontSet set;
  set.font_name = name;
  set.font_size = size;
  set.px_range  = range;

  constexpr double FONT_SCALE = 1.0;

  msdfgen::FontHandle* font_handle = msdfgen::adoptFreetypeFont(face);
  msdfgen::FontMetrics font_metrics{};
  msdfgen::getFontMetrics(font_metrics, font_handle);
  set.line_height = (font_metrics.lineHeight * FONT_SCALE / font_metrics.emSize) * size;

  auto *atlas = new FontTextureAtlas();
  if (atlas->init(face, font_handle, size, range, 1.0))
  {
    set.setAtlas(atlas);
  }
  else
  {
    Logging::ERRORS( "Renderer failed to initialise the font atlas for: ");
    Logging::ERRORS( "\t " + std::string(name));

    delete atlas;
    return nullptr;
  }

  msdfgen::destroyFont(font_handle);
  FT_Done_Face(face);

  font_sets.emplace_back(std::move(set));
  Logging::TRACE("atlas time ended");
  return &font_sets.back();
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

/**
 * Loads a font atlas from a local image and csv file.
 * @param [in] metrics The font metrics of the atlas.
 * @param [in] img_path The location of the image.
 * @param [in] csv_path The location of the CSV data used to load it.
 * @return A pointer to the newly created GLFontSet.
 */
const ASGE::GLFontSet* ASGE::GLAtlasManager::loadFontFromAtlas(
  Font::AtlasMetrics&& metrics, std::string img_path, std::string csv_path)
{
  // cast to an opengl font, so we can store a texture atlas on it
  constexpr double FONT_SCALE = 1.0;
  GLFontSet set;
  set.font_name   = metrics.id.c_str();
  set.font_size   = metrics.size;
  set.px_range    = metrics.range;
  set.line_height = (metrics.line_height * FONT_SCALE / metrics.em_size) * metrics.size;

  // open the image
  ASGE::GLTexture* atlas_image {ASGE::GLTextureCache::getInstance().createCached(img_path)};
  if(atlas_image == nullptr)
  {
    return nullptr;
  }

  atlas_image->updateMagFilter(ASGE::Texture2D::MagFilter::LINEAR);
  atlas_image->updateMinFilter(ASGE::Texture2D::MinFilter::LINEAR);
  metrics.width = atlas_image->getWidth();
  metrics.height = atlas_image->getHeight();

  // open the csv
  ASGE::FILEIO::File file;
  if (file.open(csv_path, ASGE::FILEIO::File::IOMode::READ))
  {
    ASGE::FILEIO::IOBuffer buffer = file.read();
    std::string csv {buffer.as_char(), buffer.length};
    return dynamic_cast<const ASGE::GLFontSet*>(build(atlas_image->getID(), metrics, csv));
  }
  else
  {
    const std::filesystem::path FS_PATH(csv_path);
    if (std::filesystem::is_regular_file(FS_PATH) && std::filesystem::exists(FS_PATH))
    {
      std::ifstream file_stream(FS_PATH, std::ios::in | std::ios::binary);
      if (!file_stream.is_open())
        return nullptr;

      const std::size_t& size = std::filesystem::file_size(FS_PATH);
      std::string csv_data(size, '\0');
      file_stream.read(csv_data.data(), size);
      file.close();

      return dynamic_cast<const ASGE::GLFontSet*>(build(atlas_image->getID(), metrics, csv_data));
    }
  }
  return nullptr;
}

/**
 * Converts a binary encoded image into an Atlas
 * This version of the function is unique in that it's designed for
 * images files which have been directly encoded into bytes. As these
 * images have compression and specific encodings, STBI is used to load
 * it before handing it over the AtlasManager to create the glpyhs.
 * @param [in] atlas_id The texture to use.
 * @param [in] metrics The metrics that apply to the font.
 * @param [in] csv_data The CSV file with the individual glyph data.
 * @return A pointer to the newly created GLFontSet.
 */
const ASGE::GLFontSet* ASGE::GLAtlasManager::loadFontFromAtlas(
  ASGE::Font::AtlasMetrics&& metrics, std::byte* data, std::string csv_data)
{
  // convert the image to binary format
  int bpp    = 0;
  auto image = stbi_load_from_memory(
    (unsigned char*)(data),
    static_cast<int>(kenvector_future_png_len),
    &metrics.width,
    &metrics.height,
    &bpp,
    STBI_default);

  if (image == nullptr)
  {
    return nullptr;
  }

  // create the texture to use in the atlas
  auto format = ASGE::Texture2D::Format{ static_cast<ASGE::Texture2D::Format>(bpp) };
  ASGE::GLTexture* atlas_image{ ASGE::GLTextureCache::getInstance().createCached(
    metrics.id, metrics.width, metrics.height, format, (void*)(image)) };
  if (atlas_image == nullptr)
  {
    return nullptr;
  }

  atlas_image->updateMagFilter(ASGE::Texture2D::MagFilter::LINEAR);
  atlas_image->updateMinFilter(ASGE::Texture2D::MinFilter::LINEAR);
  return dynamic_cast<const ASGE::GLFontSet*>(build(atlas_image->getID(), metrics, csv_data));
}

/**
 * Builds an atlas from a loaded texture and a CSV file.
 * @param [in] atlas_id The texture to use.
 * @param [in] metrics The metrics that apply to the font.
 * @param [in] csv The CSV file with the individual glyph data.
 * @return The newly created ASGE::Font.
 */
const ASGE::Font* ASGE::GLAtlasManager::build(int atlas_id, Font::AtlasMetrics& metrics, std::string& csv)
{
  constexpr double FONT_SCALE = 1.0;
  GLFontSet set;
  set.font_name   = metrics.id.c_str();
  set.font_size   = metrics.size;
  set.px_range    = metrics.range;
  set.line_height = (metrics.line_height * FONT_SCALE / metrics.em_size) * metrics.size;

  // Create a stringstream from line
  std::stringstream file_stream(csv);
  std::map<int, std::vector<double>> characters;

  std::string line{};
  double val = 0;
  while (std::getline(file_stream, line))
  {
    std::stringstream ss(line);
    int unicode = 0;
    ss >> unicode;
    characters[unicode] = {};

    while (std::getline(ss, line, ','))
    {
      ss >> val;
      characters[unicode].emplace_back(val);
    }
  }

  auto *atlas = new FontTextureAtlas();
  atlas->texture = atlas_id;
  atlas->width = metrics.width;
  atlas->height = metrics.height;

  // map the characters
  for(const auto & [k,v] : characters)
  {
    auto& ch     = atlas->characters[k];
    ch.Advance.x = v[0] * metrics.size;
    ch.Size.x    = ceil(v[3] * metrics.size - v[1] * metrics.size);
    ch.Size.y    = ceil(v[4] * metrics.size - v[2] * metrics.size);
    ch.Bearing.x = v[1] * metrics.size;
    ch.Bearing.y = -v[2] * metrics.size;
    ch.UV[0]     = v[5] / atlas->width;
    ch.UV[1]     = v[6] / atlas->height;
    ch.UV[2]     = v[7] / atlas->width;
    ch.UV[3]     = v[8] / atlas->height;
  }

  set.setAtlas(atlas);
  font_sets.emplace_back(std::move(set));
  return &font_sets.back();
}
