#pragma once
#include "GLIncludes.hpp"
#include "GLQuad.hpp"
#include "Sprite.hpp"
#include <bitset>
#include <list>
#include <variant>
#include <vector>

namespace ASGE
{
  class GLSprite;
  struct GLFontSet;

  /**
   *
   */
  struct GLText
  {
    const GLFontSet* font       = nullptr;
    std::string string          = "";
    GLint x                     = 0;
    GLint y                     = 0;
    GLfloat scale               = 1.0F;
    glm::vec4 colour            = { 1.0F, 1.0F, 1.0F, 1.0F };
    signed short global_z_order = 0;
    unsigned int shader_id      = 0;
    mutable int quads_drawn     = 0;
  };

  /**
   *
   */
  struct GLCharRender
  {
    const GLFontSet* font = nullptr;
    GLfloat scale         = 1.0F;
    GLfloat alpha         = 1.0F;
    GLint x               = 0;
    GLint y               = 0;
    char ch               = ' ';
  };

  /**
   *
   */
  struct AnotherRenderBatch
  {
    enum ReasonForEnding
    {
      I_DONT_KNOW       = 0,
      NO_MORE_TO_RENDER = 1,
      SHADER_CHANGE     = 2,
      TEXTURE_CHANGE    = 3,
      STATE_CHANGE      = 4,
      REASON_COUNT
    };

    GLuint start_idx      = 0;
    GLuint instance_count = 0;
    GLuint shader_id      = 0;
    GLuint texture_id     = 0;
    GLuint distance       = 0;
    RenderState* state    = nullptr;

    std::bitset<REASON_COUNT> reason = I_DONT_KNOW;
  };

  using RenderBatches = std::vector<AnotherRenderBatch>;
  using QuadList = std::vector<ASGE::RenderQuad>;
  using QuadIter = decltype(QuadList::const_iterator());

  /**
   *
   */
  struct QuadRange
  {
    explicit QuadRange(QuadIter range_begin) : QuadRange(range_begin, range_begin){};
    QuadRange(QuadIter range_begin, QuadIter range_end) : begin(range_begin), end(range_end){};
    QuadIter begin, end;
  };

  using ConstRefSprite = std::reference_wrapper<const GLSprite>;
  using primitive_data = std::variant<ConstRefSprite, GLText>;

  /**
   *
   */
  struct PrimitiveVariant
  {
    explicit PrimitiveVariant(const GLSprite& sprite) : prim{ ConstRefSprite{ sprite } } {};
    explicit PrimitiveVariant(GLText font) : prim{ font } {};
    ~PrimitiveVariant() = default;

    [[nodiscard]] int16_t getGlobalZOrder() const noexcept;
    [[nodiscard]] GLuint getTextureID() const noexcept;
    [[nodiscard]] GLuint getShader() const noexcept;
    [[nodiscard]] GLuint getQuadCount() const noexcept;
    [[nodiscard]] const GLText* asGLText() const noexcept;
    [[nodiscard]] const ConstRefSprite* asConstSprite() const noexcept;
    [[nodiscard]] bool isConstRefSprite() const noexcept
    {
      return std::holds_alternative<ConstRefSprite>(prim);
    }
    [[nodiscard]] bool isGLText() const noexcept { return std::holds_alternative<GLText>(prim); }

   private:
    primitive_data prim;
  };

  using PrimitiveList = std::list<PrimitiveVariant>;
  using PrimitiveIter = decltype(PrimitiveList::const_iterator());

  /**
   *
   */
  struct PrimitiveRange
  {
    PrimitiveRange(PrimitiveIter range_begin) : PrimitiveRange(range_begin, range_begin){};
    PrimitiveRange(PrimitiveIter range_begin, PrimitiveIter range_end) : begin(range_begin), end(range_end){};
    PrimitiveIter begin, end;
  };
}