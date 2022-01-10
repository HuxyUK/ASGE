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
#include "GLShader.hpp"
#include "GLTexture.hpp"
#include "Sprite.hpp"
#include <string>

namespace ASGE
{
  struct Tile;

  /**
   * GLSprite is the OpenGL implementation of an ASGE::Sprite
   */
	class GLSprite final: public ASGE::Sprite
	{
   public:
    GLSprite();
    ~GLSprite() override = default;
    explicit GLSprite(const ASGE::Tile&);
    bool attach(ASGE::Texture2D* texture_to_attach, AttachMode mode) noexcept override;
    bool loadTexture(const std::string& file, AttachMode mode) override;
    [[nodiscard]] Texture2D* getTexture() const override;
    [[nodiscard]] const GLTexture* asGLTexture() const noexcept;
    [[nodiscard]] const SHADER_LIB::GLShader* asGLShader() const;

  private:
		GLTexture* texture = nullptr;
    void attach(AttachMode mode);
  };
}