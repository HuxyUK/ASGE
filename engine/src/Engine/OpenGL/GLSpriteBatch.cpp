//  Copyright (c) 2021 James Huxtable. All rights reserved.
//  Adapted from the https://learnopengl.com tutorials and code.
//  This work is licensed under the terms of the MIT license.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#include <algorithm>

#include "GLAtlas.hpp"
#include "GLAtlasManager.h"
#include "GLFontSet.hpp"
#include "GLIncludes.hpp"
#include "GLModernSpriteRenderer.hpp"
#include "GLRenderBatch.hpp"
#include "GLSprite.hpp"
#include "GLSpriteBatch.hpp"

/**
 *  The constructor for the sprite batch.
 *  A sprite batch is a handy class for controlling the rendering of
 *  quads to the screen. Rendering every quad and reseting render state
 *  everytime a quad needs to be drawn is slow, this class aims to help
 *  with that problem.The constructor takes two paramets that are references
 *  to data that is shared across all the sprite rendering subsystems. This
 *  data is helpful, as it allows the sprite batch, sprite renderer and text
 *  renderer knowledge of which texture and shader is currently loaded.
 *  This info can be used to ensure shader and texture changes only
 *  happen when they are needed.
 *
 *
 *  @see ASGE::MGLSpriteRenderer
 *  @see ASGE::GLTextRenderer
 */
ASGE::GLSpriteBatch::GLSpriteBatch()
{
  quads.reserve(GLRenderConstants::MAX_BATCH_COUNT);
}

/**
 *  Sets the OpenGL render state for drawing quads.
 *  Will enable the various render states needed to draw sprites and
 *  text to the string. Changing render states is an expensive
 *  operation and the number of changes per draw cycle should be kept
 *  to a minimum. If possible, only call the begin function once every
 *  render cycle.
 */
void ASGE::GLSpriteBatch::begin()
{
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  glCullFace(GL_FRONT);
  glEnable(GL_CULL_FACE);
  glActiveTexture(GL_TEXTURE0);
}

/**
 *  Renders a sprite using the sprite batch.
 *  Depending on the active render mode the sprite may be
 *  batched for later sorting. When this happens a future render
 *  task is created as opposed to a render batch. This function
 *  also checks to see if the shared vertex data buffer is full.
 *  If this is the cause it will cause an immediate flush of all
 *  the render data to prevent buffer overruns from occuring.
 *
 *  @param spr The game sprite being rendered.
 *  @param z_order The ordering value used to create layers.
 *
 *  @see ASGE::SpriteSortMode
 */
void ASGE::GLSpriteBatch::renderSprite(const ASGE::Sprite& sprite)
{
  /// todo: thread the render quad generation process
  auto gl_sprite = dynamic_cast<const ASGE::GLSprite&>(sprite);

  // generate a render quad from sprite
  RenderQuad& quad = quads.emplace_back();
  quad.texture_id  = gl_sprite.asGLTexture()->getID();
  quad.z_order     = gl_sprite.getGlobalZOrder();
  quad.state       = &states.back();

  if (gl_sprite.asGLShader() != nullptr)
  {
    quad.shader_id = gl_sprite.asGLShader()->getShaderID();
  }
  else if (
    sprite_renderer->activeShader() != nullptr &&
    sprite_renderer->activeShader()->getShaderID() != sprite_renderer->getDefaultTextShaderID())
  {
    quad.shader_id = sprite_renderer->activeShader()->getShaderID();
  }
  else
  {
    quad.shader_id = sprite_renderer->getBasicSpriteShaderID();
  }

  sprite_renderer->quadGen(gl_sprite, quad.gpu_data);
  if (render_mode == SpriteSortMode::IMMEDIATE)
  {
    flush();
  }
}

/**
 *  Sets the sprite batch mode.
 *  The sprite batch mode will determine how sprites and text is
 *  rendered. Immediate mode for example, will cause a primitive
 *  draw for each item rendered, whereas deferred will aim to
 *  reduce the number of render state changes.
 *
 *  @param mode The new sprite batch mode to use.
 *  @see ASGE::SpriteSortMode
 */
void ASGE::GLSpriteBatch::setSpriteMode(SpriteSortMode mode)
{
  flush();
  render_mode = mode;
}

/**
 *  Gets the sprite batch mode.
 *  Returns the sprite batch's current render mode.
 *  @return The sprite sort mode
 *  @see ASGE::SpriteSortMode
 */
ASGE::SpriteSortMode ASGE::GLSpriteBatch::getSpriteMode() const
{
  return render_mode;
}

void ASGE::GLSpriteBatch::sortQuads()
{
  auto predicate = [render_mode = this->render_mode](
                     const RenderQuad& lhs, const RenderQuad& rhs) {
    auto lhs_z_order    = lhs.z_order;
    auto rhs_z_order    = rhs.z_order;
    auto lhs_texture_id = lhs.texture_id;
    auto rhs_texture_id = rhs.texture_id;
    // auto lhs_shader_id = lhs.shader_id;
    // auto rhs_shader_id = rhs.shader_id;
    // auto lhs_alpha = lhs.gpu_data.color.a;
    // auto rhs_alpha = rhs.gpu_data.color.a;

    if (render_mode == SpriteSortMode::TEXTURE)
    {
      return (lhs_texture_id < rhs_texture_id);
    }

    if (render_mode == SpriteSortMode::BACK_TO_FRONT)
    {
      // if z order is lower
      return (lhs_z_order < rhs_z_order) ||
             (lhs_z_order == rhs_z_order && lhs_texture_id < rhs_texture_id);
    }
    else
    {
      // if z order is higher
      return (lhs_z_order > rhs_z_order) ||
             (lhs_z_order == rhs_z_order && lhs_texture_id < rhs_texture_id);
    }
  };

  std::stable_sort(quads.begin(), quads.end(), predicate);
  return;
}

/**
 *  Flushes all the render tasks queued in the sprite batch.
 *  If the rendering requires sorting then it will process
 *  any future batches by sorting them and converting them into batches.
 *  Once the futures are converted they can then be drawn by flushing
 *  all the batches. Lastly, the sprite batch will signal shared vertex
 *  data to be emptied and the render queue to be cleared. There should
 *  be no future or render batches remaining after this function call.
 */
void ASGE::GLSpriteBatch::flush()
{
  if (!quads.empty())
  {
    sortQuads();
    QuadRange upload_range{ quads.cbegin(), std::prev(quads.cend())};
    while(upload_range.begin != quads.cend())
    {
      const auto last_uploaded_quad = sprite_renderer->upload(upload_range);
      auto&& batches = generateRenderBatches({upload_range.begin, last_uploaded_quad});
      current_draw_count += sprite_renderer->render(std::move(batches));
      upload_range.begin = std::next(last_uploaded_quad);
    }
    quads.clear();
  }

  sprite_renderer->clearActiveRenderState();
  states.clear();
}

/**
 *
 * @return
 */
std::vector<ASGE::AnotherRenderBatch>
ASGE::GLSpriteBatch::generateRenderBatches(const QuadRange& range)
{
  auto batch_begin = range.begin;
  auto batch_end   = range.begin;

  auto should_end = [&batch_begin, &batch_end]() {
    return batch_begin->texture_id != batch_end->texture_id ||
           batch_begin->shader_id  != batch_end->shader_id  ||
           batch_begin->distance   != batch_end->distance   ||
           batch_begin->state      != batch_end->state;
  };

  auto get_reason = [&batch_begin, &batch_end, &range]() {
    std::bitset<AnotherRenderBatch::REASON_COUNT> reason;
    if (batch_end >= range.end)
    {
      reason.set(AnotherRenderBatch::NO_MORE_TO_RENDER);
      return reason;
    }
    if (batch_begin->texture_id != batch_end->texture_id)
    {
      reason.set(AnotherRenderBatch::TEXTURE_CHANGE);
    }
    if (batch_begin->shader_id != batch_end->shader_id ||
        batch_begin->distance  != batch_end->distance)
    {
      reason.set(AnotherRenderBatch::SHADER_CHANGE);
    }
    if (batch_begin->state != batch_end->state)
    {
      reason.set(AnotherRenderBatch::STATE_CHANGE);
    }

    return reason;
  };

  std::vector<AnotherRenderBatch> batches;
  auto create_batch = [&](int64_t count) {
    auto& batch          = batches.emplace_back(AnotherRenderBatch{});
    batch.reason         = get_reason();
    batch.start_idx      = static_cast<GLuint>(std::distance(range.begin, batch_begin));
    batch.instance_count = static_cast<GLuint>(count);
    batch.texture_id     = batch_begin->texture_id;
    batch.shader_id      = batch_begin->shader_id;
    batch.distance       = batch_begin->distance;
    batch.state          = batch_begin->state;
  };

  do
  {
    if (should_end())
    {
      create_batch(std::distance(batch_begin, batch_end));
      batch_begin = batch_end;
    }
  } while (batch_end++ != range.end);

  create_batch(std::distance(batch_begin, range.end) + 1);
  return batches;
}

/**
 *  Ends the sprite batch routine. In effect this will flush
 *  any rendering that has not been completed, unbind the
 *  textures and then the vertex array. This is only to be called
 *  at the end of each frame render as it will cause issues
 *  and slowdown if called mid-render cycle. Instead prefer the
 *  use of flush which maintains the sprite batch setup.
 *
 *  @see flush
 */
void ASGE::GLSpriteBatch::end()
{
  flush();
  current_draw_count = 0;
}

void ASGE::GLSpriteBatch::renderText(const ASGE::Text& text)
{
  if (!text.validFont())
  {
    return;
  }

  GLCharRender render_char;
  auto x_pos_begin = text.getPosition().x;
  const auto& font = dynamic_cast<const ASGE::GLFontSet&>(text.getFont());

  float x = text.getPosition().x;
  float y = text.getPosition().y;

  for (auto character : text.getString())
  {
    if (character == '\n')
    {
      x = x_pos_begin;
      y += static_cast<float>(text.getFont().line_height) * text.getScale();
      continue;
    }

    RenderQuad& quad = quads.emplace_back();
    quad.texture_id  = font.getAtlas()->getTextureID();
    quad.shader_id   = sprite_renderer->getDefaultTextShaderID();
    quad.z_order     = text.getZOrder();
    quad.distance    = font.px_range * text.getScale();
    quad.state       = &states.back();

    // the character we want to render
    render_char.scale = text.getScale();
    render_char.ch    = character;
    render_char.x     = static_cast<GLint>(x);
    render_char.y     = static_cast<GLint>(y);
    render_char.font  = &font;
    render_char.alpha = text.getOpacity();

    sprite_renderer->createCharQuad(render_char, text.getColour(), quad.gpu_data);
    x += font.pxWide(render_char.ch, render_char.scale);
  }

  if (render_mode == SpriteSortMode::IMMEDIATE)
  {
    flush();
  }
}

void ASGE::GLSpriteBatch::saveState(RenderState&& state)
{
  states.emplace_back(std::move(state));
}
