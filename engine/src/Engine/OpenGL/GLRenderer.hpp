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

#include "Engine/Renderer.hpp"
#include "GLSpriteBatch.hpp"
#include <Engine/Camera.hpp>
#include <iostream>
#include <memory>
#include <vector>

struct GLFWwindow;
namespace ASGE
{
	namespace SHADER_LIB
	{
		class  Shader;
		class  GLShader;
	}

  struct Font;
  class  Sprite;
  class  Input;
  class  CGLSpriteRenderer;
  class GLAtlasManager;

  /**
   * The heart and soul of the OpenGL ASGE renderer. Creates
   * the text and sprite renderers and is responsible for setting
   * up the window with a valid OpenGL context. All rendering
   * calls come through the renderer and are proxied on to the
   * corresponding OpenGL version specific renderer.
   */
  class GLRenderer final: public Renderer
	{
	 public:
    enum class RenderLib
    {
      UNINITIALISED = 0,
      GL_MODERN = 1,
      GL_LEGACY = 2
    }; static RenderLib RENDER_LIB;

	public:

    SHADER_LIB::Shader* getShader() override;

		GLRenderer();
		~GLRenderer() override;
    GLRenderer(const GLRenderer&) = delete;
    GLRenderer operator=(const GLRenderer&) = delete;

    bool init() override;
		bool exit() override;
		void setDefaultTextColour(const Colour& rgb) override;
		void preRender() override;
		void swapBuffers() override;
		void renderDebug();
		
		std::unique_ptr<Sprite> createUniqueSprite() override;
		Sprite* createRawSprite() override;
		GLFWwindow* getWindow();
		const glm::mat4& getProjectionMatrix() const;

		// Inherited via Renderer
		int     loadFont(const char * font, int pt) override;
		int     loadFontFromMem(const char* name, const unsigned char* data, unsigned int size, int pt) override;
		void    setFont(int id) override;
		void    setWindowTitle(const char * str) override;
		void    setSpriteMode(SpriteSortMode sort_mode) override;
		void    setWindowedMode(GameSettings::WindowMode window_mode) override;
		void    setClearColour(ASGE::Colour rgb) override;
		const   Font& getDefaultFont() const override;

    // TODO create central shader storage, this is bad design
    SHADER_LIB::Shader* initPixelShader(std::string shader_source) override;
    static std::vector<SHADER_LIB::GLShader> shaders;
    void setActiveShader(SHADER_LIB::Shader* shader) override;
    SHADER_LIB::Shader* findShader(int shader_handle) override;
    SHADER_LIB::GLShader* getActiveShader();

    ASGE::Texture2D* createNonCachedTexture(int width, int height, ASGE::Texture2D::Format pixel_format, void* data) override;
    ASGE::Texture2D* createNonCachedTexture(const std::string& path) override;
    ASGE::Texture2D* createCachedTexture(std::string id, int width, int height, ASGE::Texture2D::Format pixel_format, void* data) override;
    ASGE::Texture2D* createCachedTexture(const std::string& path) override;

    const Font& getFont(int idx) const override;
    void renderText(const Text& string) override;
    void renderText(Text&& string) override;
    void setProjectionMatrix(float min_x, float max_x, float min_y, float max_y) override;
    void setProjectionMatrix(const Camera::CameraView& view) override;
    void renderSprite(const Sprite& sprite) override;
    void render(ASGE::Texture2D& texture,  int x, int y) override;

    ASGE::Viewport getViewport() const override;
    void setViewport(const ASGE::Viewport& viewport) override;
    void setRenderTarget(const ASGE::RenderTarget* /*unused*/) override;
    [[nodiscard]] float windowHeight() const noexcept override;
    [[nodiscard]] float windowWidth() const noexcept override;

   private:
    std::unique_ptr<Input> inputPtr() override;
    void postRender() override;
    void updateMonitorInfo(GLFWmonitor* monitor);

   private:
    GLSpriteBatch batch{};
    glm::mat4 projection_matrix{};
    std::unique_ptr<CGLSpriteRenderer> sprite_renderer{};
    std::unique_ptr<GLAtlasManager> text_renderer{};

    std::array<int32_t, 2> desktop_res{ 0, 0 };
    unsigned int target_width  = 640;
    unsigned int target_height = 480;
    int16_t desktop_refresh    = 60;
    Colour debug_text_colour   = COLOURS::DEEPPINK;
    GLFWwindow* window         = nullptr;
    GLuint projection_ubo      = -1;

    void centerWindow();
  };
}