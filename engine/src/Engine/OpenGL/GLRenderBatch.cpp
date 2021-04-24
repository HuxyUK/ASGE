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

#include "GLRenderBatch.hpp"
#include "GLAtlas.hpp"
#include "GLFontSet.hpp"
#include "GLSprite.hpp"

template<class T>
struct always_false : std::false_type
{
};
int16_t ASGE::PrimitiveVariant::getGlobalZOrder() const noexcept
{
  return std::visit(
    [](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, ConstRefSprite>)
        return arg.get().getGlobalZOrder();
      else if constexpr (std::is_same_v<T, GLText>)
        return arg.global_z_order;
      else
        static_assert(always_false<T>::value, "non-exhaustive visitor!");
      return (signed short)(0);
    },
    prim);
}

GLuint ASGE::PrimitiveVariant::getTextureID() const noexcept
{
  return std::visit(
    [](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, ConstRefSprite>)
        return arg.get().asGLTexture()->getID();
      else if constexpr (std::is_same_v<T, GLText>)
        return arg.font->getAtlas()->getTextureID();
      else
        static_assert(always_false<T>::value, "non-exhaustive visitor!");
      return GLuint{ 0 };
    },
    prim);
}

GLuint ASGE::PrimitiveVariant::getQuadCount() const noexcept
{
  return std::visit(
    [](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, ConstRefSprite>)
        return GLuint{ 1 };
      else if constexpr (std::is_same_v<T, GLText>)
        return static_cast<GLuint>(arg.quads_drawn);
      else
        static_assert(always_false<T>::value, "non-exhaustive visitor!");
      return GLuint{ 0 };
    },
    prim);
}

GLuint ASGE::PrimitiveVariant::getShader() const noexcept
{
  return std::visit(
    [](auto&& arg) -> GLuint {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, ConstRefSprite>)
      {
        if (auto gl_shader = arg.get().asGLShader())
        {
          return gl_shader->getShaderID();
        }
      }
      else if constexpr (std::is_same_v<T, GLText>)
      {
        return arg.shader_id;
      }
      else
      {
        static_assert(always_false<T>::value, "non-exhaustive visitor!");
      }
      return 0;
    },
    prim);
}

const ASGE::GLText* ASGE::PrimitiveVariant::asGLText() const noexcept
{
  return std::get_if<GLText>(&prim);
}

const ASGE::ConstRefSprite* ASGE::PrimitiveVariant::asConstSprite() const noexcept
{
  return std::get_if<ConstRefSprite>(&prim);
}