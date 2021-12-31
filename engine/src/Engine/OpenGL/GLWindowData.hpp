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

#pragma once

namespace ASGE
{
  class GLRenderer;
  class GLInput;
  class Resolution;

  struct WindowData
  {
    ASGE::GLRenderer* renderer = nullptr;
    ASGE::GLInput* input = nullptr;
    ASGE::Resolution* resolution = nullptr;
  };
}  // namespace ASGE
