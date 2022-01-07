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

#pragma once
#include "GLQuad.hpp"
#include "GLRenderBatch.hpp"
#include "Text.hpp"
#include "GLRenderState.hpp"
#include <vector>

namespace ASGE {

	class CGLSpriteRenderer;
	class GLAtlasManager;
	class GLSprite;

	/**
	*  A sprite batch class designed for OpenGL.
	*  The sprite batch manages the current render state, including shaders
	*  and is responsible for tracking loaded textures, queuing render tasks
	*  and sorting of render tasks. It utilises the SpriteRenderer and
	*  TextRenderer, both of which have their own shaders and vertex buffers. 
	*  scale, flip, recolour etc. 2D games are made up of these. */
	class GLSpriteBatch
  {
   public:
    friend class GLRenderer;

   public:
    GLSpriteBatch();
    ~GLSpriteBatch() = default;

    void begin();
    void renderSprite(const ASGE::Sprite&);
    void renderText(const ASGE::Text&);

    void flush();
    void end();
    void setSpriteMode(SpriteSortMode mode);
    SpriteSortMode getSpriteMode() const;

   private:
    mutable unsigned int current_draw_count = 0;
    CGLSpriteRenderer* sprite_renderer      = nullptr;
    SpriteSortMode render_mode              = SpriteSortMode::BACK_TO_FRONT;

    std::vector<ASGE::AnotherRenderBatch>
    generateRenderBatches(const QuadRange& range);
    void sortQuads();
    void saveState(RenderState&& state);
    QuadList quads;
    std::list<RenderState> states{};
  };
}