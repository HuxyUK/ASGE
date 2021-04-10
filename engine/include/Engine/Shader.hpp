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

//! @file Shader.hpp
//! @brief Namespace @ref ASGE::SHADER_LIB, Class @ref ASGE::SHADER_LIB::Shader

#ifndef ASGE_SHADER_HPP
#define ASGE_SHADER_HPP

#include "NonCopyable.hpp"
#include "Value.hpp"
#include <array>
#include <map>
#include <memory>
#include <string>

/**
 * @namespace ASGE::SHADER_LIB
 * @brief The ASGE Shader Library
 */
namespace ASGE::SHADER_LIB
{
  using uniformB    = ASGE::ValueBase;
  using uniform_ptr = std::unique_ptr<uniformB>;
  using Uniform1b   = bool;
  using Uniform1f   = float;
  using Uniform1i   = int;
  using Uniformv2b  = std::array<bool, 2>;
  using Uniformv2f  = std::array<float, 2>;
  using Uniformv3f  = std::array<float, 3>;
  using Uniformv4f  = std::array<float, 4>;
  using UniformMat2 = std::array<std::array<float, 2>, 2>;
  using UniformMat4 = std::array<std::array<float, 4>, 4>;

  /**
   *  @brief a shader instance.
   *  The shader is used in the rendering pipeline.
   */
  class Shader : public NonCopyable
  {
   private:
    std::map<std::string, uniform_ptr> uniforms;

   public:
    Shader()          = default;
    virtual ~Shader() = default;

    /**
     * Retrieves a uniform by its name.
     * @param name The name of the uniform to access.
     * @return The uniform handle, for updating GPU data.
     */
    uniformB* getUniform(const std::string& name);

    /**
     * Shorthand for uniform lookup type.
     */
    using uniform_record = std::pair<std::string, uniform_ptr>;

    /**
     * Returns the list of uniforms attached to this shader.
     * @return The uniform handles.
     */
    [[nodiscard]] decltype(uniforms)& getUniforms() { return uniforms; }

    /**
     * Returns the list of uniforms attached to this shader.
     * @return The uniform handles.
     */
    [[nodiscard]] const decltype(uniforms)& getUniforms() const { return uniforms; }
  };
} // namespace ASGE::SHADER_LIB
#endif // ASGE_SHADER_HPP
