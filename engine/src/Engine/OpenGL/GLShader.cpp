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

#include <iostream>
#include "Logger.hpp"
#include "GLShader.hpp"

ASGE::SHADER_LIB::GLShader::~GLShader()
{
  if ((this->ID != 0U) && (glfwGetCurrentContext() != nullptr))
  {
    glDeleteProgram(this->ID);
  }
}

ASGE::SHADER_LIB::GLShader& ASGE::SHADER_LIB::GLShader::use()
{
  glUseProgram(this->ID);
  return *this;
}

ASGE::SHADER_LIB::GLShader& ASGE::SHADER_LIB::GLShader::use(GLuint& pipeline)
{
  if (shader_type == GL_VERTEX_SHADER)
  {
    glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, this->ID);
  }

  if (shader_type == GL_FRAGMENT_SHADER)
  {
    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, this->ID);
  }

  return *this;
}

bool ASGE::SHADER_LIB::GLShader::compile(GLenum type, const GLchar* source)
{
  if (source != nullptr)
  {
    this->ID = glCreateShaderProgramv(type, 1, &source);
    if (checkCompileErrors(this->ID, "PROGRAM"))
    {
      return false;
    }

    shader_type = type;
    mapUniforms();
    return true;
  }

  return false;
}

bool ASGE::SHADER_LIB::GLShader::compile(
  const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
  GLuint sVertex = 0, sFragment = 0, gShader = 0;

  auto cleanup = [&]()
  {
      // Delete the shaders as they're linked into our program now and no longer necessary
      if(sVertex)        glDeleteShader (sVertex);
      if(sFragment)      glDeleteShader (sFragment);
      if(geometrySource) glDeleteShader (gShader);
  };

  // Vertex GLShader
  if (vertexSource != nullptr)
  {
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, nullptr);
    glCompileShader(sVertex);
    if (checkCompileErrors(sVertex, "VERTEX"))
    {
      cleanup();
      return false;
    }
  }

  // Fragment GLShader
  if (fragmentSource != nullptr)
  {
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, nullptr);
    glCompileShader(sFragment);
    if (checkCompileErrors(sFragment, "FRAGMENT"))
    {
      cleanup();
      return false;
    }
  }

  // If geometry shader source code is given, also compile geometry shader
  if (geometrySource != nullptr)
  {
    gShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gShader, 1, &geometrySource, nullptr);
    glCompileShader(gShader);
    if (checkCompileErrors(gShader, "GEOMETRY"))
    {
      cleanup();
      return false;
    }
  }

  // GLShader Program
  this->ID = glCreateProgram();

  if (sVertex)
    glAttachShader(this->ID, sVertex);
  if (sFragment)
    glAttachShader(this->ID, sFragment);
  if (geometrySource)
    glAttachShader(this->ID, gShader);

  glLinkProgram(this->ID);
  if (checkCompileErrors(this->ID, "PROGRAM"))
  {
    return false;
  }

  mapUniforms();
  return true;
}

bool ASGE::SHADER_LIB::GLShader::loadShaderFromFile(
  const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile)
{
  // 1. Retrieve the vertex/fragment source code from filePath
  std::string vertex_code;
  std::string fragment_code;
  std::string geometry_code;
  try
  {
    // Open files
    std::ifstream vertex_shader_file(vShaderFile);
    std::ifstream fragment_shader_file(fShaderFile);
    std::stringstream v_shader_stream;
    std::stringstream f_shader_stream;

    // Read file's buffer contents into streams
    v_shader_stream << vertex_shader_file.rdbuf();
    f_shader_stream << fragment_shader_file.rdbuf();

    // close file handlers
    vertex_shader_file.close();
    fragment_shader_file.close();

    // Convert stream into string
    vertex_code   = v_shader_stream.str();
    fragment_code = f_shader_stream.str();

    // If geometry shader path is present, also load a geometry shader
    if (gShaderFile != nullptr)
    {
      std::ifstream geometry_shader_file(gShaderFile);
      std::stringstream g_shader_stream;
      g_shader_stream << geometry_shader_file.rdbuf();
      geometry_shader_file.close();
      geometry_code = g_shader_stream.str();
    }
  }
  catch (std::exception& e)
  {
    Logging::ERRORS("Failed to read shader files");
    Logging::ERRORS(e.what());
  }

  const GLchar* v_shader_code = vertex_code.c_str();
  const GLchar* f_shader_code = fragment_code.c_str();
  const GLchar* g_shader_code = geometry_code.c_str();

  // 2. Now create shader object from source code
  return compile(v_shader_code, f_shader_code, gShaderFile != nullptr ? g_shader_code : nullptr);
}

void ASGE::SHADER_LIB::GLShader::setBool(Program program, Location location, Uniform1b value)
{
  glProgramUniform1f(program, location, static_cast<GLfloat>(value));
}

void ASGE::SHADER_LIB::GLShader::setFloat(Program program, Location location, Uniform1f value)
{
  glProgramUniform1f(program, location, value);
}

void ASGE::SHADER_LIB::GLShader::setInteger(Program program, Location location, Uniform1i value)
{
  glProgramUniform1i(program, location, value);
}

void ASGE::SHADER_LIB::GLShader::setVector2b(
  Program program, Location location, const Uniformv2b& value)
{
  glProgramUniform2ui(program, location, static_cast<GLuint>(value[0]), static_cast<GLuint>(value[1]));
}

void ASGE::SHADER_LIB::GLShader::setVector2f(
  Program program, Location location, const Uniformv2f& values)
{
  glProgramUniform2f(program, location, values[0], values[1]);
}

void ASGE::SHADER_LIB::GLShader::setVector3f(
  Program program, Location location, const Uniformv3f& values)
{
  glProgramUniform3f(program, location, values[0], values[1], values[2]);
}

void ASGE::SHADER_LIB::GLShader::setVectorv3(
  Program program, Location location, const glm::vec3& values)
{
  glProgramUniform3f(program, location, values.x, values.y, values.z);
}

void ASGE::SHADER_LIB::GLShader::setVector4f(
  Program program, Location location, const Uniformv4f& values)
{
  glProgramUniform4f(program, location, values[0], values[1], values[2], values[3]);
}

void ASGE::SHADER_LIB::GLShader::setMatrix4(
  Program program, Location location, const UniformMat4& values)
{
  glm::mat4 matrix = glm::make_mat4(values.data()->data());
  glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ASGE::SHADER_LIB::GLShader::setMatrix2(
  Program program, Location location, const UniformMat2& values)
{
  glm::mat2 matrix = glm::make_mat2(values.data()->data());
  glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, glm::value_ptr(matrix));
}

int ASGE::SHADER_LIB::GLShader::getShaderID() const
{
  return this->ID;
}

bool ASGE::SHADER_LIB::GLShader::operator==(const GLShader& rhs) const
{
  return ID == rhs.getShaderID();
}

bool ASGE::SHADER_LIB::GLShader::operator==(GLuint _id) const
{
  return ID == _id;
}

int ASGE::SHADER_LIB::GLShader::getLocation(const GLchar* name) const
{
  return glGetAttribLocation(this->ID, name);
}

bool ASGE::SHADER_LIB::GLShader::checkCompileErrors(GLuint object, const std::string& type)
{
  GLint success = 0;
  std::array<GLchar, 1024> info_log{};
  if (type != "PROGRAM")
  {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
      glGetShaderInfoLog(object, 1024, nullptr, info_log.data());
      std::cout << "| ERROR::SHADER: compile-time error: Type: " << type << "\n"
                << info_log.data() << "\n -- --------------------------------------------------- -- "
                << std::endl;

      return true;
    }

    return false;
  }

  glGetProgramiv(object, GL_LINK_STATUS, &success);
  if (success == 0)
  {
    glGetProgramInfoLog(object, 1024, nullptr, info_log.data());
    std::cout << "| ERROR::GLShader: Link-time error: Type: " << type << "\n"
              << info_log.data() << "\n -- --------------------------------------------------- -- "
              << std::endl;

    return true;
  }

  return false;
}

ASGE::SHADER_LIB::GLShader::GLShader(ASGE::SHADER_LIB::GLShader&& rhs) noexcept
: ID(rhs.ID), shader_type(rhs.shader_type)
{
  getUniforms()     = std::move(rhs.getUniforms());
  rhs.ID            = 0;
}

ASGE::SHADER_LIB::GLShader&
ASGE::SHADER_LIB::GLShader::operator=(ASGE::SHADER_LIB::GLShader&& rhs) noexcept
{
  this->ID          = rhs.ID;
  getUniforms()     = std::move(rhs.getUniforms());
  this->shader_type = rhs.shader_type;
  rhs.ID            = 0;
  return *this;
}

void ASGE::SHADER_LIB::GLShader::mapUniforms()
{
  GLint i     = 0;
  GLint count = 0;

  GLint  size = 0; // size of the variable
  GLenum type = 0; // type of the variable (float, vec3 or mat4, etc)

  const GLsizei BUF_SIZE = 32;  // maximum name length
  GLchar        name[BUF_SIZE]; // variable name in GLSL
  GLsizei       length = 0;    // name length

  Logging::TRACE("Processing Shader #" + std::to_string(this->ID));
  glGetProgramiv(this->ID, GL_ACTIVE_UNIFORMS, &count);
  Logging::TRACE("Active Uniforms: " + std::to_string(count));

  for (i = 0; i < count; i++)
  {
    glGetActiveUniform(this->ID, (GLuint)i, BUF_SIZE, &length, &size, &type, name);
    auto              location = glGetUniformLocation(this->ID, name);
    std::stringstream ss;
    ss << "\t Uniform #" << i << " "
       << "Type: " << type << " "
       << "Name: " << name;

    Logging::TRACE(ss.str());

    switch (type)
    {
      case GL_FLOAT:
      {
        getUniforms().emplace(
          uniform_record(name, new UniformGL<Uniform1f>(this->ID, location, &GLShader::setFloat)));
        break;
      }
      case GL_INT:
      case GL_SAMPLER_2D:
      case GL_SAMPLER_2D_ARRAY:
      {
        getUniforms().emplace(uniform_record(
          name, new UniformGL<Uniform1i>(this->ID, location, &GLShader::setInteger)));
        break;
      }
      case GL_BOOL_VEC2:
      {
        getUniforms().emplace(uniform_record(
          name, new UniformGL<Uniformv2b>(this->ID, location, &GLShader::setVector2b)));
        break;
      }

      case GL_FLOAT_VEC2:
      {
        getUniforms().emplace(uniform_record(
          name, new UniformGL<Uniformv2f>(this->ID, location, &GLShader::setVector2f)));
        break;
      }

      case GL_FLOAT_VEC3:
      {
        getUniforms().emplace(uniform_record(
          name, new UniformGL<Uniformv3f>(this->ID, location, &GLShader::setVector3f)));
        break;
      }

      case GL_FLOAT_VEC4:
      {
        getUniforms().emplace(uniform_record(
          name, new UniformGL<Uniformv4f>(this->ID, location, &GLShader::setVector4f)));
        break;
      }

      case GL_FLOAT_MAT2:
      {
        getUniforms().emplace(uniform_record(
          name, new UniformGL<UniformMat2>(this->ID, location, &GLShader::setMatrix2)));
        break;
      }
      case GL_FLOAT_MAT4:
      {
        getUniforms().emplace(uniform_record(
          name, new UniformGL<UniformMat4>(this->ID, location, &GLShader::setMatrix4)));
        break;
      }

      default:
      {
        Logging::WARN("Unsupported shader uniform detected");
        break;
      }
    }
  }

  Logging::TRACE("Uniforms successfully mapped:");

  int counter = 0;
  for (auto& uni : getUniforms())
  {
    std::stringstream ss;
    ss << "\t Uniform #" << counter++ << " \"" << uni.first << "\"";
    Logging::TRACE(ss.str());
  }
}
