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


//! @file Tile.hpp
//! @brief
//! Class @ref ASGE::Tile,
//! Enum @ref ASGE::Sprite::FlipFlags,
//! Enum @ref ASGE::Sprite::SourceRectIndex

#ifndef PYASGE_TILE_HPP
#define PYASGE_TILE_HPP

#include "Colours.hpp"
namespace ASGE
{
  /**
   *  @brief An ASGE supported tile.
   *
   *  Tiles, whilst sharing much of the same functionality as the ASGE::Sprite
   *  are conceptually quite different. Instead they are used to build more
   *  complex scenes and game maps. Because of this their position is
   *  normally dictated by the map itself and typically consists of using rows,
   *  columns and tile sizes to determine their exact positioning.
   *
   *  The aim of this struct is to easily allow this kind of functionality to
   *  exist within ASGE. Now instead of having to construct instances of
   *  Sprites with their own positional data, you can use a Tile instead.
   *
   *  When rendering a Tile you will need to provide the XY position that it's
   *  to appear in World Space. Remember that by default origins in ASGE
   *  for positions (not rotations) are calculated from the top-left.
   *
   *  Mozilla's developer documentation (https://developer.mozilla.org/en-US/docs/Games/Techniques/Tilemaps)
   *  does a great job of introducing tile maps and how conceptually they
   *  operate. This might help if you're unsure about when you would use a
   *  Tile instead of a Sprite.
   *
   *  @image html "https://developer.mozilla.org/en-US/docs/Games/Techniques/Tilemaps/tile_atlas.png" "Mozilla Developer Docs" width="100%"
   *
   * Example:
   * <example>
   * @code
   *   // create a tile with a texture and source rectangle
   *   auto& tile = tiles.emplace_back();
   *   tile.texture = renderer->createCachedTexture("/data/img/tile_sheet.png");
   *   tile.width = 64
   *   tile.height = 64
   *   tile.src_rect = {0, 0, 64, 64};
   *
   *   // assuming a 2D array
   *   for (int row=0; row < map.length; ++row) {
   *     for (int col=0; col < map.length; ++col) {
   *       renderer->render(tiles[row][col],
   *       ASGE::Point2D{col * map.tile_width, row * map.tile_height});
   *     }
   *   }
   * @endcode
   * </example>
   */
  struct Tile
  {
    std::array<float, 4> src_rect{ 0, 0, 1, 1 }; /**< Source Rectangle. The source rectangle used for sampling. */
    ASGE::Colour tint{ ASGE::COLOURS::WHITE };   /**< Tint. Used to tint the colour of the tile. */
    ASGE::Texture2D* texture{ nullptr };         /**< Texture. The source rectangle used for sampling. */
    float rotation{ 0.0F };                      /**< Rotation. Rotation to apply in radians. */
    float opacity{ 1.0F };                       /**< Opacity. Controls the alpha channel i.e. transparency. */
    int width{ 32 };                             /**< Width. How wide to render the tile. */
    int height{ 32 };                            /**< Height. How tall to render the tile. */
    int16_t z{ 0 };                              /**< Z. The rendering order to apply to the tile. */
  };
}  // namespace ASGE

#endif // PYASGE_TILE_HPP
