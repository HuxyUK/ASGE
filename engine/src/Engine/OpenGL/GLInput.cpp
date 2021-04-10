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

#include "GLInput.hpp"
#include "GLIncludes.hpp"
#include "GLRenderer.hpp"
#include <glm/gtc/matrix_transform.hpp>

bool ASGE::GLInput::init(Renderer* renderer)
{
  auto* glrenderer = dynamic_cast<GLRenderer*>(renderer);
  if (glrenderer == nullptr)
  {
    return false;
  }

  projection_matrix = &glrenderer->getProjectionMatrix();
  window            = glrenderer->getWindow();
  glfwSetWindowUserPointer(window, this);

  auto kFunc = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto event      = std::make_shared<KeyEvent>();
    event->key      = key;
    event->scancode = scancode;
    event->action   = action;
    event->mods     = mods;
    static_cast<GLInput*>(glfwGetWindowUserPointer(window))->sendEvent(EventType::E_KEY, event);
  };

  auto mButtonEvent = [](GLFWwindow* window, int button, int action, int mods) {
    auto event    = std::make_shared<ClickEvent>();
    event->button = button;
    event->action = action;
    event->mods   = mods;

    auto* input = static_cast<GLInput*>(glfwGetWindowUserPointer(window));

    input->getCursorPos(event->xpos, event->ypos);
    input->sendEvent(EventType::E_MOUSE_CLICK, event);
  };

  auto mMoveEvent = [](GLFWwindow* window, double xpos, double ypos) {
    auto event  = std::make_shared<MoveEvent>();
    event->xpos = xpos;
    event->ypos = ypos;

    auto* input = static_cast<GLInput*>(glfwGetWindowUserPointer(window));

    input->getCursorPos(event->xpos, event->ypos);
    input->sendEvent(EventType::E_MOUSE_MOVE, event);
  };

  auto mScrollEvent = [](GLFWwindow* window, double xoffset, double yoffset) {
    auto event     = std::make_shared<ScrollEvent>();
    event->xoffset = xoffset;
    event->yoffset = yoffset;
    static_cast<GLInput*>(glfwGetWindowUserPointer(window))
      ->sendEvent(EventType::E_MOUSE_SCROLL, event);
  };

  glfwSetKeyCallback(glrenderer->getWindow(), kFunc);
  glfwSetMouseButtonCallback(glrenderer->getWindow(), mButtonEvent);
  glfwSetCursorPosCallback(glrenderer->getWindow(), mMoveEvent);
  glfwSetScrollCallback(glrenderer->getWindow(), mScrollEvent);
  return true;
}

void ASGE::GLInput::update() {}

void ASGE::GLInput::getCursorPos(double& xpos, double& ypos) const
{
  glfwGetCursorPos(window, &xpos, &ypos);
  unProjectCursor(xpos, ypos);
}

void ASGE::GLInput::unProjectCursor(double& xpos, double& ypos) const
{
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glm::vec4 viewportdata = glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]);
  glm::vec3 pos          = glm::vec3(xpos, viewportdata.w - ypos, 0.0F);
  glm::mat4 model        = glm::mat4(1.0F);

  auto unprojected = glm::unProject(pos, model, *projection_matrix, viewportdata);
  xpos             = unprojected[0];
  ypos             = unprojected[1];
}

void ASGE::GLInput::setCursorMode(ASGE::MOUSE::CursorMode mode)
{
  switch (mode)
  {
    case ASGE::MOUSE::CursorMode::NORMAL:
      glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      break;

    case ASGE::MOUSE::CursorMode::HIDDEN:
      glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      break;

    case ASGE::MOUSE::CursorMode::LOCKED:
      glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      break;
  }
}

ASGE::GamePadData ASGE::GLInput::getGamePad(int idx) const
{
  /* NOTE: This is based on the current GLFW 3.2.1 version
   *  however, there are a great deal of improvements happening
   *  in 3.3,  some of which which alter the gamepad API and
   *  introduce haptic feedback. We should look to upgrade when
   *  it's finally released.
   */

  if (glfwJoystickPresent(idx) == 0)
  {
    GamePadData data(idx, std::string("not connected").c_str(), 0, nullptr, 0, nullptr);
    data.is_connected = false;
    return data;
  }

  const auto* name = glfwGetJoystickName(idx);

  int axis_count           = 0;
  int button_count         = 0;
  const auto* axis_state   = glfwGetJoystickAxes(idx, &axis_count);
  const auto* button_state = glfwGetJoystickButtons(idx, &button_count);

  GamePadData data(idx, name, axis_count, axis_state, button_count, button_state);
  data.is_connected = true;
  return data;
}

void ASGE::GLInput::setCursorPos(double xpos, double ypos) const
{
  glfwSetCursorPos(this->window, xpos, ypos);
}
