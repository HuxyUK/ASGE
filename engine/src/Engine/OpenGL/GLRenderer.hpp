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

#include "Renderer.hpp"
#include "GLSpriteBatch.hpp"
#include "Camera.hpp"
#include "GLWindowData.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <Resolution.hpp>

struct GLFWwindow;
namespace ASGE
{
	namespace SHADER_LIB
	{
		class  Shader;
		class  GLShader;
	}

  struct Font;
  struct Tile;
  class  Sprite;
  class  CGLSpriteRenderer;
  class  GLAtlasManager;

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

    bool init(const ASGE::GameSettings& settings) override;
    bool exit() override;
    void setDefaultTextColour(const Colour& rgb) override;
    void preRender() override;
    void swapBuffers() override;
    void renderDebug(int fps);

    std::unique_ptr<Sprite> createUniqueSprite() override;
    Sprite* createRawSprite() override;
    GLFWwindow* getWindow();

    // Inherited via Renderer
    const Font& getDefaultFont() const override;
    const Font* loadFont(const char * font, int size, double range) override;
    const Font* loadFontFromMem(const char* name, const unsigned char* data, unsigned int len, int size, double range) override;
    const Font* loadFontAtlas(Font::AtlasMetrics&& metrics, const std::string& img_path, const std::string& csv_path) override;
    void  setFont(int id) override;
    void  setWindowTitle(const char * str) override;
    void  setSpriteMode(SpriteSortMode sort_mode) override;
    void  setWindowedMode(GameSettings::WindowMode window_mode) override;
    void  setClearColour(ASGE::Colour rgb) override;
    void  setBaseResolution(int width, int height, Resolution::Policy policy) override;

    // TODO create central shader storage, this is bad design
    SHADER_LIB::Shader* initPixelShader(std::string shader_source) override;
    static std::vector<SHADER_LIB::GLShader> shaders;
    void setActiveShader(SHADER_LIB::Shader* shader) override;
    SHADER_LIB::Shader* findShader(int shader_handle) override;
    SHADER_LIB::GLShader* getActiveShader();

    ASGE::Texture2D* createCachedTextureArray(std::string id, int width, int height, ASGE::Texture2D::Format format, void* data, int count) override;
    ASGE::Texture2D* createNonCachedTexture(int width, int height, ASGE::Texture2D::Format pixel_format, void* data) override;
    ASGE::Texture2D* createNonCachedMSAATexture(int width, int height, ASGE::Texture2D::Format pixel_format) override;
    ASGE::Texture2D* createNonCachedTexture(const std::string& path) override;
    ASGE::Texture2D* createCachedTexture(std::string id, int width, int height, ASGE::Texture2D::Format pixel_format, void* data) override;
    ASGE::Texture2D* createCachedTexture(const std::string& path) override;

    const Font& getFont(int idx) const override;
    const Resolution& resolution() const override;
    void setResolutionPolicy(Resolution::Policy) override;

    void setProjectionMatrix(float min_x, float max_x, float min_y, float max_y) override;
    void setProjectionMatrix(const Camera::CameraView& view) override;
    void render(const Sprite& sprite) override;
    void render(const Text& string) override;
    void render(Text&& string) override;
    void render(const ASGE::Tile& tile, const ASGE::Point2D& xy) override;
    void render(ASGE::Texture2D &texture, std::array<float, 4> rect, const Point2D &xy, int width, int height, int16_t z) override;

    ASGE::Viewport getViewport() const override;
    void setViewport(const ASGE::Viewport &viewport) override;
    void setRenderTarget(RenderTarget* render_target) override;
    [[nodiscard]] int windowHeight() const noexcept override;
    [[nodiscard]] int windowWidth() const noexcept override;
    [[nodiscard]] std::tuple<int32_t,int32_t,int16_t> screenRes() override;
    [[nodiscard]] const Resolution& getResolutionInfo() const;

   private:
    std::unique_ptr<Input> inputPtr() override;
    void allocateDebugTexture();
    void centerViewPort(const ASGE::Viewport& viewport);
    void centerWindow();
    void fillViewPort(const Viewport& viewport);
    void fitViewPort(const Viewport& viewport);
    void postRender() override;
    void saveState();
    void updateMonitorInfo(GLFWmonitor* monitor);

   private:
    GLSpriteBatch batch{};
    Resolution resolution_info{};
    WindowData window_data{};
    Resolution::Policy resolution_policy{ Resolution::Policy::MAINTAIN };
    std::unique_ptr<CGLSpriteRenderer> sprite_renderer{};
    std::unique_ptr<GLAtlasManager> text_renderer{};
    GLFWwindow* window{ nullptr };
  };
}  // namespace ASGE