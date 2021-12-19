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

#ifndef SHADER_H
#define SHADER_H

#include <functional>
#include <string>
#include "GLIncludes.hpp"
#include "Shader.hpp"

namespace ASGE
{
  class GLModernSpriteRenderer;
  class GLAtlasManager;

  namespace SHADER_LIB
  {
    using Program  = int;
    using Location = int;

    template<typename T>
    class UniformGL : public Value<T>
    {
     public:
      UniformGL(
        Program program_in, Location location_in,
        std::function<void(Program, Location, const T&)> gl_fnc) :
        program(program_in),
        location(location_in), fnc(gl_fnc)
      {
      }

      void set(std::any object) override
      {
        *std::any_cast<T*>(this->get()) = std::any_cast<T>(object);
        fnc(program, location, std::any_cast<T>(object));
      }

     private:
      Program program;
      Location location;
      std::function<void(Program, Location, const T&)> fnc = nullptr;
    };

    // General purpose shader object. Compiles from file, generates
    // compile/link-time error messages and hosts several utility
    // functions for easy management.
    class GLShader : public Shader
    {
     public:
      GLShader() = default;
      ~GLShader() override;

      GLShader(GLShader& rhs) = delete;
      GLShader& operator=(const GLShader& rhs) = delete;

      GLShader(GLShader&& rhs) noexcept;
      GLShader& operator=(GLShader&& rhs) noexcept;

      bool operator==(const GLShader& s) const;
      bool operator==(GLuint _id) const;

      // Utility functions
      [[nodiscard]] int getShaderID() const;
      int getLocation(const GLchar* name) const;
      bool loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);

      static void setBool(Program program, Location location, bool value);
      static void setFloat(Program program, Location location, float value);
      static void setInteger(Program program, Location location, int value);
      static void setVector2b(Program program, Location location, const Uniformv2b& values);
      static void setVector2f(Program program, Location location, const Uniformv2f& values);
      static void setVector3f(Program program, Location location, const Uniformv3f& values);
      static void setVector4f(Program program, Location location, const Uniformv4f& values);
      static void setMatrix2(Program program, Location location, const UniformMat2& values);
      static void setMatrix4(Program program, Location location, const UniformMat4& values);
      static void setVectorv3(Program program, Location location, const glm::vec3& values);

      // Compiles the shader from given source code
      bool compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr);
      bool compile(GLenum type, const GLchar* source);

      // Checks if compilation or linking failed and if so, print the error logs
      static bool checkCompileErrors(GLuint object, const std::string& type);

      friend class ::ASGE::GLAtlasManager;
      friend class ::ASGE::GLModernSpriteRenderer;

      // Sets the current shader as active
      GLShader& use();
      GLShader& use(GLuint& pipeline);

     private:
      void mapUniforms();

      // State
      GLuint ID{};
      GLenum shader_type = GL_VERTEX_SHADER;
    };
  } // namespace SHADER_LIB
} // namespace ASGE
#endif
