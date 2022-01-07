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

#ifndef ASGE_GLQUAD_H
#define ASGE_GLQUAD_H

#include <array>
#include "GLConstants.hpp"
#include "GLIncludes.hpp"

namespace ASGE
{
  /**
   * A GPUQuad is the data the GPU requires in order to render a texture to the
   * screen. It consists of the transform data (matrix), colour and UV data.
   * These quads are directly copied from CPU to GPU memory when batches
   * are uploaded.
   */
  struct GPUQuad
  {
    GPUQuad() = default;
    GPUQuad(const GPUQuad&);
    GPUQuad& operator=(const GPUQuad&);
    GPUQuad& operator=(GPUQuad&&) noexcept;
    GPUQuad(GPUQuad&&) noexcept;
    ~GPUQuad() = default;

    glm::mat4 position = glm::mat4{ 1 };
    static constexpr const glm::vec2 PADDING{ 5, 5 };
    glm::vec4 color = glm::vec4{ 1, 1, 1, 1 };
    std::array<glm::vec4, GLRenderConstants::VERTEX_PER_QUAD> uv_data = {
      glm::vec4{ 0.0F, 1.0F, PADDING },
      glm::vec4{ 0.0F, 0.0F, PADDING },
      glm::vec4{ 1.0F, 0.0F, PADDING },
      glm::vec4{ 1.0F, 1.0F, PADDING }
    };
  };

  static constexpr GLsizei QUAD_STORAGE_SIZE = sizeof(GPUQuad);

  static_assert(
    (GLRenderConstants::MAX_BATCH_COUNT * QUAD_STORAGE_SIZE) % 64 == 0,
    "BATCH COUNT IS NOT DIVISIBLE BY 64. BUFFER RANGES CAN NOT MAP!");

  //  base alignment    // aligned offset
  //  float value;      // 4               // 0
  //  vec3 vector;      // 16              // 16  (must be multiple of 16 so 4->16)
  //  mat4 matrix;      // 16              // 32  (column 0)
  //  // 16             // 48  (column 1)
  //  // 16             // 64  (column 2)
  //  // 16             // 80  (column 3)
  //  float values[3];  // 16              // 96  (values[0])
  //  // 16             // 112 (values[1])
  //  // 16             // 128 (values[2])
  //  bool boolean;     // 4               // 144
  //  int integer;      // 4               // 148

  //  std::array<glm::vec4, GLRenderConstants::VERTEX_PER_QUAD> uv_data =
  //    { glm::vec4{ 0.0f, 1.0f, padding_t}, glm::vec4{ 0.0f, 0.0f, padding_t },
  //      glm::vec4{ 1.0f, 0.0f, padding_t}, glm::vec4{ 1.0f, 0.0f, padding_t },
  //      glm::vec4{ 1.0f, 1.0f, padding_t}, glm::vec4{ 0.0f, 1.0f, padding_t } };

  class RenderState;
  struct RenderQuad
  {
    RenderQuad() = default;
    RenderQuad(const RenderQuad&);
    RenderQuad& operator=(const RenderQuad&);
    RenderQuad(RenderQuad&&) noexcept;
    RenderQuad& operator=(RenderQuad&&) noexcept;
    ~RenderQuad() = default;

    GPUQuad gpu_data {};
    GLuint  shader_id  = 0;
    GLuint  texture_id = 0;
    GLshort z_order    = 0;
    GLfloat distance   = 0;
    RenderState* state = nullptr;
  };

  enum BufferState : unsigned int
  {
    UPLOAD_OKAY,
    BUFFER_FULL = 1
  };
}  // namespace ASGE

#endif // ASGE_GLQUAD_H
