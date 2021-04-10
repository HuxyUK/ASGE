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

#ifndef ASGE_GLCONSTANTS_H
#define ASGE_GLCONSTANTS_H

#include "GLIncludes.hpp"
namespace ASGE::GLRenderConstants
  {
    static constexpr int MAX_BATCH_COUNT = 310000; // 310689;
    static constexpr int UVS_PER_QUAD = 16; // 8UVS + 32Bytes padding
    static constexpr int VERTEX_PER_QUAD = 4;

    static constexpr GLsizei UV_STORAGE_SIZE = sizeof(float) * UVS_PER_QUAD * MAX_BATCH_COUNT;
    static constexpr GLsizei MATRIX_STORAGE_SIZE = sizeof(glm::mat4) * MAX_BATCH_COUNT;
    static constexpr GLsizei COLOUR_STORAGE_SIZE = sizeof(glm::vec4) * MAX_BATCH_COUNT;

    static constexpr GLuint PROJECTION_UBO_BIND = 1;
    static constexpr GLuint OFFSET_UBO_BIND = 2;

    /// LEGACY RENDERER
    static constexpr GLuint QUAD_DATA_SSBO_BIND = 10;
    static constexpr GLuint QUAD_UBO_LIMIT = 400;
    static constexpr GLuint QUAD_DATA_UBO_BIND = 10;

    static constexpr GLfloat QUAD_VERTICES[] =
      { 0.0F, 1.0F, 0.0F, 0.0F,
        1.0F, 0.0F, 1.0F, 1.0F };

    static constexpr GLubyte QUAD_INDICIES[] =
      { 0, 1, 2, 0, 2, 3 };
  }  // namespace ASGE::GLRenderConstants
#endif // ASGE_GLCONSTANTS_H
