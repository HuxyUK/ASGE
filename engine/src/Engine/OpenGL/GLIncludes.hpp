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

#pragma once
#ifndef ASGE_DEBUGPR2INTER_H
#define ASGE_DEBUGPR2INTER_H

#include "Logger.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <string>

namespace ASGE
{
  #ifdef GL_CHECK_ERRORS
  inline void ClearGLErrors(std::string&& user_msg = " ")
  {
    while (auto error = glad_glGetError())
    {
      Logging::ERRORS(user_msg);
      std::stringstream ss;
      ss << "GLERROR: " << std::to_string(error);

      Logging::ERRORS(ss.str());
    }
  }
  #else
  inline void ClearGLErrors(std::string&& user_msg = " "){};
  #endif // GL_CHECK_ERRORS

  template<typename GLFnc, typename... Params>
  auto constexpr GLCMD(GLFnc fnc, Params&&... params)
  {
    auto result = fnc(std::forward<Params>(params)...);
    ASGE::ClearGLErrors();
    return result;
  }

  template<typename GLFnc, typename... Params>
  auto GLMSG(std::string log_msg, GLFnc fnc, Params&&... params)
  {
    auto result = fnc(std::forward<Params>(params)...);
    ASGE::ClearGLErrors(std::move(log_msg));
    return result;
  }

  template<typename GLFnc, typename... Params>
  inline void GLVCMD(GLFnc fnc, Params&&... params)
  {
    fnc(std::forward<Params>(params)...);
    ASGE::ClearGLErrors();
  }

  template<typename GLFnc, typename... Params>
  auto GLVMSG(std::string log_msg, GLFnc fnc, Params&&... params)
  {
    fnc(std::forward<Params>(params)...);
    ASGE::ClearGLErrors(std::move(log_msg));
  }
}  // namespace ASGE

#endif