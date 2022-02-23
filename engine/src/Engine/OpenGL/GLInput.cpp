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

#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>

#include "FileIO.hpp"
#include "GLIncludes.hpp"
#include "GLInput.hpp"
#include "GLRenderer.hpp"

bool ASGE::GLInput::init(Renderer* renderer)
{
  auto* glrenderer = dynamic_cast<GLRenderer*>(renderer);
  if (glrenderer == nullptr)
  {
    return false;
  }

  window            = glrenderer->getWindow();

  auto kFunc = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto event      = std::make_shared<KeyEvent>();
    event->key      = key;
    event->scancode = scancode;
    event->action   = action;
    event->mods     = mods;

    auto* input = static_cast<WindowData*>(glfwGetWindowUserPointer(window))->input;
    input->sendEvent(EventType::E_KEY, event);
  };

  auto mButtonEvent = [](GLFWwindow* window, int button, int action, int mods) {
    auto event    = std::make_shared<ClickEvent>();
    event->button = button;
    event->action = action;
    event->mods   = mods;

    auto* input = static_cast<WindowData*>(glfwGetWindowUserPointer(window))->input;
    input->getCursorPos(event->xpos, event->ypos);
    input->sendEvent(EventType::E_MOUSE_CLICK, event);
  };

  auto mMoveEvent = [](GLFWwindow* window, double xpos, double ypos) {
    auto event  = std::make_shared<MoveEvent>();
    event->xpos = xpos;
    event->ypos = ypos;

    auto* input = static_cast<WindowData*>(glfwGetWindowUserPointer(window))->input;
    input->getCursorPos(event->xpos, event->ypos);
    input->sendEvent(EventType::E_MOUSE_MOVE, event);
  };

  auto mScrollEvent = [](GLFWwindow* window, double xoffset, double yoffset) {
    auto event     = std::make_shared<ScrollEvent>();
    event->xoffset = xoffset;
    event->yoffset = yoffset;
    auto* input = static_cast<WindowData*>(glfwGetWindowUserPointer(window))->input;
    input->sendEvent(EventType::E_MOUSE_SCROLL, event);
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
  auto& resolution = static_cast<WindowData*>(glfwGetWindowUserPointer(window))->resolution;

  auto view  = resolution->view;
  constexpr float min = std::numeric_limits<decltype(RenderQuad::z_order)>::min();
  constexpr float max = std::numeric_limits<decltype(RenderQuad::z_order)>::max();
  auto pm = glm::ortho<float>(view.min_x, view.max_x, view.max_y, view.min_y, min, max);

  auto& viewport = resolution->viewport;

  glm::vec4 viewportdata = glm::vec4(viewport.x, viewport.y, viewport.w, viewport.h);
  glm::vec3 pos          = glm::vec3(xpos, viewportdata.y * 2 + viewportdata.w - ypos, 0.0F);
  glm::mat4 model        = glm::mat4(1.0F);

  auto unprojected = glm::unProject(pos, model, pm, viewportdata);
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
  if (glfwJoystickIsGamepad(idx) == 0)
  {
    GamePadData data(idx, std::string("not connected").c_str(), nullptr, nullptr);
    data.is_connected = false;
    return data;
  }

  const auto* name = glfwGetGamepadName(idx);
  GLFWgamepadstate state;
  glfwGetGamepadState(idx, &state);
  const auto* axis_state   = state.axes;
  const auto* button_state = state.buttons;

  GamePadData data(idx, name, axis_state, button_state);
  data.is_connected = true;
  return data;
}

ASGE::GamePadData ASGE::GLInput::getGamePad() const
{
  for(int idx=0; idx<GLFW_JOYSTICK_LAST; ++idx)
  {
    if(glfwJoystickIsGamepad(idx))
    {
      const auto* name = glfwGetGamepadName(idx);
      GLFWgamepadstate state;
      glfwGetGamepadState(idx, &state);
      const auto* axis_state   = state.axes;
      const auto* button_state = state.buttons;
      GamePadData data(idx, name, axis_state, button_state);
      data.is_connected = true;
      return data;
    }
  }
  return ASGE::GamePadData(0, std::string("not connected").c_str(), nullptr, nullptr);
}

std::vector<ASGE::GamePadData> ASGE::GLInput::getGamePads() const
{
  std::vector<ASGE::GamePadData> gamepads;
  for(int idx=0; idx<GLFW_JOYSTICK_LAST; ++idx)
  {
    auto data = getGamePad(idx);
    if(data.is_connected)
    {
      gamepads.emplace_back(std::move(data));
    }
  }
  return gamepads;
}

void ASGE::GLInput::setCursorPos(double xpos, double ypos) const
{
  glfwSetCursorPos(this->window, xpos, ypos);
}

void ASGE::GLInput::updateGamePadMappings(const std::filesystem::path &mappings_file)
{
  std::string file_contents;
  ASGE::FILEIO::File file;
  if(file.open(mappings_file.string(), ASGE::FILEIO::File::IOMode::READ))
  {
    const auto buffer = file.read();
    file_contents = std::string(buffer.data.get(), buffer.length);
  }

  else if(!exists(mappings_file))
  {
    std::ifstream file(mappings_file.c_str());
    file_contents = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  }

  if (file_contents.empty())
    return;

  glfwUpdateGamepadMappings(file_contents.c_str());
}