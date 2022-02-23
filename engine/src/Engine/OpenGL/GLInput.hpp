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

#include <filesystem>
#include "Input.hpp"
#include "GLIncludes.hpp"

struct GLFWwindow;

namespace ASGE
{
  class Renderer;
  class GLInput : public Input
  {
   public:
    GLInput()           = default;
    ~GLInput() override = default;

    // Inherited via Input
    bool init(Renderer* renderer) override;
    void update() override;
    void getCursorPos(double& x, double& y) const override;
    void setCursorMode(ASGE::MOUSE::CursorMode mode) override;
    void setCursorPos(double x, double y) const override;
    [[nodiscard]] GamePadData getGamePad(int idx) const override;
    [[nodiscard]] GamePadData getGamePad() const override;
    [[nodiscard]] std::vector<GamePadData> getGamePads() const override;
    void updateGamePadMappings(const std::filesystem::path &mappings_file) override;

  private:
    GLFWwindow* window                 = nullptr;
    const glm::mat4* projection_matrix = nullptr;
    void unProjectCursor(double& xpos, double& ypos) const;
  };
}  // namespace ASGE
