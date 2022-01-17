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

#include "Camera.hpp"
#include "GLAtlasManager.h"
#include "GLFontSet.hpp"
#include "GLInput.hpp"
#include "GLLegacySpriteRenderer.hpp"
#include "GLModernSpriteRenderer.hpp"
#include "GLRenderTarget.hpp"
#include "GLRenderer.hpp"
#include "GLSprite.hpp"
#include "GLTextureCache.hpp"
#include "Logger.hpp"
#include "OpenGL/Shaders/GLShaders.vs"
#include "Tile.hpp"
#include <cmath>
#include <memory>

namespace
{
  void windowSizeCallback(GLFWwindow* window, int width, int height)
  {
    Logging::TRACE("Window resized event");
  }

  void framebufferSizeCallback(GLFWwindow* window, int width, int height)
  {
    Logging::TRACE("Framebuffer resized event");

    auto* renderer = static_cast<ASGE::WindowData*>(glfwGetWindowUserPointer(window))->renderer;
    auto* resolution = static_cast<ASGE::WindowData*>(glfwGetWindowUserPointer(window))->resolution;

    if(renderer->getWindowMode() != ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN)
    {
      resolution->window[0] = width;
      resolution->window[1] = height;
    }

    renderer->setViewport(ASGE::Viewport{0,0, resolution->base[0], resolution->base[1]});
    renderer->setProjectionMatrix(0, 0, static_cast<float>(resolution->base[0]), static_cast<float>(resolution->base[1]));
  }
}  // namespace

/// used to find out easily which renderer is in use
ASGE::GLRenderer::RenderLib ASGE::GLRenderer::RENDER_LIB = ASGE::GLRenderer::RenderLib::UNINITIALISED;

/**
 *  The constructor for the renderer class.
 *  A specalised version of a render that utilises the OpenGL rendering
 *  subsystem. It is responsible for window creation and management,
 *  the rendering of sprites and text. It also manages the sprite batch
 *  which is used to reduce the number of primitive draw calls where
 *  possible. This particular type of renderer uses GLEW. The active
 *  texture and shader represent the currently loaded texture/shader
 *  on the GPU and can be used to reduce render state changes.
 *
 *  @see ASGE::RenderBatch
 *  @see ASGE::Renderer::RenderLib
 */
ASGE::GLRenderer::GLRenderer()
{
  shaders.reserve(20);
  window_data.renderer = this;
  window_data.resolution = &resolution_info;
}

/**
 *  The destructor for the renderer class.
 *  The rendersystem destruction involves the termination of the
 *  game window. The texture cache used for storing pre-loaded
 *  sprites needs to be reset and the loaded textures freed on the
 *  GPU subsystem.
 */
ASGE::GLRenderer::~GLRenderer()
{
  GLTextureCache::getInstance().reset();
  glfwTerminate();
}

/**
*  The initialisation of the renderer class.
*  The initialisation of the renderer involves creation of the
*  GLFW window context, it's version and profile. The window
*  will be created using the resolution specified. At this point
*  the window mode will be set and the sprite and text rendering
*  systems also created. After this function completes, the
*  window and rendering systems will be ready to draw.

*  @param [in] width The windows width.
*  @param [in] height The windows height.
*  @param [in] settings Windowed or Fullscreen setup.
*
*  @return True if initialisation was a success.
*/
bool ASGE::GLRenderer::init(const ASGE::GameSettings& settings)
{
  // Window size
  resolution_info.window[0] = settings.window_width;
  resolution_info.window[1] = settings.window_height;

  // Init GLFW
  if (glfwInit() == GLFW_FALSE)
  {
    return false;
  }

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  using GLVERSION = std::pair<int, int>;
  std::map<GLVERSION, std::function<void()>, std::greater<>> my_map;

  my_map[GLVERSION(4, 5)] = [&]() {sprite_renderer = std::make_unique<GLModernSpriteRenderer>();};
  my_map[GLVERSION(3, 3)] = [&]() {sprite_renderer = std::make_unique<GLLegacySpriteRenderer>();};

  for (auto& item : my_map)
  {
    Logging::DEBUG("Starting OPENGL Window");
    Logging::DEBUG(
      "Attempting GLVERSION: " +
      std::to_string(item.first.first) + "." +
      std::to_string(item.first.second));

    // try to create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, item.first.first);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, item.first.second);

    GLFWwindow* msaa_window = glfwCreateWindow(640, 480, "MSAA Check", nullptr, nullptr);
    if (msaa_window != nullptr)
    {
      Logging::INFO(
              "Launching Window with GLVERSION: " +
              std::to_string(item.first.first) + "." +
              std::to_string(item.first.second) + " Support");

      glfwMakeContextCurrent(msaa_window);
      if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
      {
        Logging::ERRORS("Failed to initialise GLAD");
        throw std::runtime_error("gladLoadGLLoader failed");
        return false;
      }

      Logging::INFO("=> " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));

      // Calculate MAX MSAA
      GLint max_samples = 0;
      glGetIntegerv ( GL_MAX_SAMPLES, &max_samples );
      Logging::INFO(std::string("Max Supported Samples: ") + std::to_string(max_samples));
      msaa(std::clamp(settings.msaa_level, 4, max_samples));
      glfwWindowHint(GLFW_SAMPLES, msaa());

      // Create the actual window and close the temporary one
      auto [width, height] = resolution_info.window;
      window = glfwCreateWindow(width, height, "ASGE", nullptr, msaa_window);
      glfwMakeContextCurrent(window);
      glfwDestroyWindow(msaa_window);

      // Initialise the GL Sprite Renderer
      item.second();
      RENDER_LIB = sprite_renderer->getRenderLib();
      break;
    }
  }

  glfwSetWindowUserPointer(window, &window_data);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  updateMonitorInfo(glfwGetPrimaryMonitor());
  centerWindow();

  GLTextureCache::getInstance().renderer = this;
  setWindowedMode(settings.mode);
  setWindowTitle(settings.game_title.c_str());
  glfwShowWindow(this->window);
  magFilter(settings.mag_filter);
  glGetIntegerv(GL_VIEWPORT, &resolution_info.viewport.x);

  text_renderer = std::make_unique<GLAtlasManager>();
  text_renderer->init();
  sprite_renderer->init();
  batch.sprite_renderer = sprite_renderer.get();

  switch(settings.vsync)
  {
    case GameSettings::Vsync::ENABLED:
      glfwSwapInterval( 1); break;
    case GameSettings::Vsync::ADAPTIVE:
      glfwSwapInterval(-1); break;
    default:
      glfwSwapInterval(0);
  }

  const auto& cls = clearColour();
  glClearColor(cls.r, cls.g, cls.b, 1.0F);
  glEnable(GL_MULTISAMPLE);
  ClearGLErrors(__PRETTY_FUNCTION__);
  allocateDebugTexture();
  setProjectionMatrix(0, 0, resolution_info.window[0], resolution_info.window[1]);
  //setProjectionMatrix(0, 0, resolution_info.base[0], resolution_info.base[1]);
  glfwSetWindowSizeCallback(window, windowSizeCallback);
  return true;
}

void ASGE::GLRenderer::allocateDebugTexture()
{
  // Create one pixel texture
  auto *blank_texture = createCachedTexture("__asge__debug__texture__", 1, 1, GLTexture::RGBA, nullptr);
  auto *pixel_buffer = blank_texture->getPixelBuffer();
  const static std::array<std::byte, 4> PIXEL{
    static_cast<std::byte>(255),  // R
    static_cast<std::byte>(152),  // G
    static_cast<std::byte>(180),  // B
    static_cast<std::byte>(128)}; // A
  memcpy(pixel_buffer->getPixelData(), PIXEL.data(), sizeof(std::byte) * PIXEL.size());
  pixel_buffer->upload(0);
  ClearGLErrors(__PRETTY_FUNCTION__);
}

/**
 *  Checks to see if the renderer wants to exit.
 *  Using the glfw window handle, the renderer will return
 *  true if the the window is tring to close.
 *
 *  @return true if the renderer should exit
 */
bool ASGE::GLRenderer::exit()
{
  return glfwWindowShouldClose(window) != 0;
}

/**
 *  Sets the default text colour for rendering.
 *  If no colour is specified the default colour will be used
 *  to render text.
 *
 *  @param [in] rgb The default colour of the text to be drawn.
 */
void ASGE::GLRenderer::setDefaultTextColour(const Colour& rgb)
{
  defTextColour() = rgb;
}

/**
 *  Creates a OpenGL sprite.
 *  Depending on the rendering system used, sprites need additional
 *  information regarding their texture and loading systems. This
 *  generates a self-managed version of a sprite that used for
 *  OpenGL rendering. The user will not need to free the memory used.
 *
 *  @return A newly allocated sprite.
 */
std::unique_ptr<ASGE::Sprite> ASGE::GLRenderer::createUniqueSprite()
{
  return std::make_unique<GLSprite>();
}

/**
 *  Creates a OpenGL sprite.
 *  Depending on the rendering system used, sprites need additional
 *  information regarding their texture and loading systems. This
 *  generates a non-managed version of a sprite that used for
 *  OpenGL rendering. The user will need to free the memory used.
 *
 *  @return A newly allocated sprite, that requires manual deletion.
 */
ASGE::Sprite* ASGE::GLRenderer::createRawSprite()
{
  return new GLSprite;
}

/**
 *  Gets the window.
 *  Returns the GLFWwindow that is used to manage the rendering.
 *  @return The GLFW window pointer.
 *  @see GLFWwindow
 */
GLFWwindow* ASGE::GLRenderer::getWindow()
{
  return window;
}

/**
 *  Sets the pre-render state.
 *  Before rendering can begin, the current window needs
 *  clearing and the sprite batch render states need to be
 *  set. This is a one time every tick only requirement.
 */
void ASGE::GLRenderer::preRender()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  saveState();
  batch.begin();
}

/**
 *  Swaps the buffers.
 *  Takes the sprites and text drawn to the back buffer
 *  and presents it to the window. It will also signal
 *  GLFW to check for events and send them accordingly.
 */
void ASGE::GLRenderer::swapBuffers()
{
  // Swap the buffers
  glfwSwapBuffers(window);

  // Check and call events
  glfwPollEvents();
}

/**
 *  Debug render.
 *  Used to describe the current render mode and the number
 *  of primitive draws that happen throughout the render
 *  cycle. It will always cause any existing data to be flushed
 *  and drawn to the screen first.
 */
void ASGE::GLRenderer::renderDebug(int fps)
{
  batch.flush();

  int width = 0;
  int height = 0;

  if (windowMode() == GameSettings::WindowMode::BORDERLESS_FULLSCREEN)
  {
    width  = resolution_info.desktop[0];
    height = resolution_info.desktop[1];
  }
  else
  {
    width  = resolution_info.window[0];
    height = resolution_info.window[1];
  }

  auto original_vp         = resolution_info.viewport;
  auto original_projection = resolution_info.view;
  resolution_info.viewport = {0,0,width,height};
  setProjectionMatrix({ 0, 0, static_cast<float>(width), static_cast<float>(height) });

  std::string fps_str = std::to_string(fps);
  constexpr auto POS_X    = 25.F;
  constexpr auto POS_Y    = 34.F; // renderer->getDefaultFont().line_height;

  auto text = ASGE::Text{ getFont(0) };
  text.setString(fps_str);
  //text.setColour({ 1.0F, 0.2F, 0.75F });
  text.setColour(COLOURS::DEEPPINK);
  text.setPosition({ POS_X, POS_Y });
  text.setScale(0.5);
  render(std::move(text));

  std::string debug_string;
  switch (batch.getSpriteMode())
  {
    case ASGE::SpriteSortMode::IMMEDIATE:
    {
      debug_string += "IMMEDIATE \n";
      break;
    }
    case ASGE::SpriteSortMode::TEXTURE:
    {
      debug_string += "TEXTURE SORT \n";
      break;
    }
    case ASGE::SpriteSortMode::FRONT_TO_BACK:
    {
      debug_string += "FRONT TO BACK \n";
      break;
    }
    case ASGE::SpriteSortMode::BACK_TO_FRONT:
    {
      debug_string += "BACK TO FRONT \n";
      break;
    }
  }

  debug_string += (std::string("DRAW COUNT: ") + std::to_string(batch.current_draw_count));

  Text debug_text = { getFont(0), debug_string.c_str(), static_cast<int>(POS_X), 52, ASGE::COLOURS::PINK };
  debug_text.setScale(0.25);
  batch.renderText(debug_text);
  batch.flush();

  // restore the original settings
  resolution_info.viewport = original_vp;
  glViewport(original_vp.x, original_vp.y, original_vp.w, original_vp.h);
  setProjectionMatrix(original_projection);
}

/**
 *  Initialises the input system.
 *  In GLFW the input system is linked to the window. This
 *  function will create a new instance of GLinput, initialise
 *  it and return a memory managed copy of it. Due to the implicit
 *  nature of the input needing the renderer, initialisation
 *  happens here.
 *
 *  @return An memory managed input system.
 *  @see ASGE::GLInput
 */
std::unique_ptr<ASGE::Input> ASGE::GLRenderer::inputPtr()
{
  auto input = std::make_unique<GLInput>();
  if (input->init(this))
  {
    window_data.input = input.get();
    return input;
  }

  return nullptr;
}

/**
 *  Post render function.
 *  Tidies up the render state and any data that still hasn't been
 *  flushed. In turn it will call the render batch's end function
 *  to ensure all the remaining sprites and text is drawn.
 */
void ASGE::GLRenderer::postRender()
{
  batch.end();
  sprite_renderer->setActiveShader(nullptr);
}

/**
 * @brief Stretches the viewport to fit within the window.
 *
 * @Details Given a viewport, the function will scale the viewport
 * to fill the window, whilst maintaining its original proportions
 * in relation to the base resolution. For example, a 50% wide viewport
 * will be scaled to ensure 50% of the display.
 *
 * @note This function will stretch anything rendered inside the viewport.
 * @param viewport[in] The requested viewport size.
 */
void ASGE::GLRenderer::fillViewPort(const ASGE::Viewport& viewport)
{
  auto window_width =
    getWindowMode() == ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN
      ? resolution_info.desktop[0]
      : resolution_info.window[0];

  auto window_height =
    getWindowMode() == ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN
      ? resolution_info.desktop[1]
      : resolution_info.window[1];

  auto scale_x = static_cast<float>(window_width)  / static_cast<float>(resolution_info.base[0]);
  auto scale_y = static_cast<float>(window_height) / static_cast<float>(resolution_info.base[1]);
  auto scale_w = static_cast<float>(viewport.w) / static_cast<float>(resolution_info.base[0]);
  auto scale_h = static_cast<float>(viewport.h) / static_cast<float>(resolution_info.base[1]);

  ASGE::Viewport vp_modified
    { static_cast<int>(static_cast<float>(viewport.x) * scale_x),
      static_cast<int>(static_cast<float>(viewport.y) * scale_y),
      static_cast<int>(std::round(window_width * scale_w)),
      static_cast<int>(std::round(window_height * scale_h)) };

  glViewport(vp_modified.x, vp_modified.y, vp_modified.w, vp_modified.h);
  resolution_info.viewport = vp_modified;
}

/**
 * @brief Attempts to scale the viewport to the game window.
 *
 * @Details Given a viewport, the function will attempt to frame
 * it appropriately within the monitor's primary desktop resolution,
 * using padding where needed and always trying to maintain the
 * original requested aspect ratio.
 *
 * @param viewport[in] The requested viewport size.
 */
void ASGE::GLRenderer::fitViewPort(const ASGE::Viewport& viewport)
{
  auto window_width =
    getWindowMode() == ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN
      ? resolution_info.desktop[0]
      : resolution_info.window[0];

  auto window_height =
    getWindowMode() == ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN
      ? resolution_info.desktop[1]
      : resolution_info.window[1];

  float design_aspect_ratio = resolution_info.getBaseAspectRatio();
  float screen_aspect_ratio =
    getWindowMode() == ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN
      ? resolution_info.getDesktopAspectRatio()
      : resolution_info.getWindowAspectRatio();

  float scalar =
    screen_aspect_ratio > design_aspect_ratio
      ? static_cast<float>(window_height) / static_cast<float>(resolution_info.base[1])
      : static_cast<float>(window_width)  / static_cast<float>(resolution_info.base[0]);

  auto padding_x = std::round((static_cast<float>(window_width)  - static_cast<float>(resolution_info.base[0]) * scalar) * 0.5F);
  auto padding_y = std::round((static_cast<float>(window_height) - static_cast<float>(resolution_info.base[1]) * scalar) * 0.5F);
  auto offset_x  = (static_cast<float>(viewport.x) * scalar) + padding_x;
  auto offset_y  = (static_cast<float>(viewport.y) * scalar) + padding_y;
  auto vp_width  = static_cast<float>(viewport.w) * scalar;
  auto vp_height = static_cast<float>(viewport.h) * scalar;

  ASGE::Viewport vp_modified
    { static_cast<int>(offset_x),
      static_cast<int>(offset_y),
      static_cast<int>(vp_width),
      static_cast<int>(vp_height) };

  glViewport(vp_modified.x, vp_modified.y, vp_modified.w, vp_modified.h);
  resolution_info.viewport = vp_modified;
}

/**
 * @brief Centers the viewport in the window.
 *
 * @Details Given a viewport, this function will center it in the
 * middle of the screen. As it does not scale the viewport contents
 * any contents too large to fit in the window will not be shown.
 *
 * @param viewport[in] The requested viewport size.
 */
void ASGE::GLRenderer::centerViewPort(const ASGE::Viewport& viewport)
{
  // find the width of the screen
  auto window_width =
    getWindowMode() == ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN
      ? resolution_info.desktop[0]
      : resolution_info.window[0];

  auto window_height =
    getWindowMode() == ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN
      ? resolution_info.desktop[1]
      : resolution_info.window[1];

  auto offset_x = static_cast<float>(window_width  - resolution_info.base[0]) * 0.5F;
  auto offset_y = static_cast<float>(window_height - resolution_info.base[1]) * 0.5F;

  ASGE::Viewport vp_modified
    { static_cast<int>(static_cast<float>(viewport.x) + offset_x),
      static_cast<int>(static_cast<float>(viewport.y) + offset_y),
      static_cast<int>(viewport.w),
      static_cast<int>(viewport.h) };

  glViewport(vp_modified.x, vp_modified.y, vp_modified.w, vp_modified.h);
  resolution_info.viewport = vp_modified;
}

/**
 *  Sets the window mode.
 *  It is possible to display the game in a window, fullscreen or
 *  even borderless mode. This function will set the screen mode. At
 *  present it will always use the main monitor as the destination
 *  for the game.
 *
 *  @param [in] mode_request The window mode requested.
 *  @see ASGE::GLRenderer::WindowMode
 */
void ASGE::GLRenderer::setWindowedMode(GameSettings::WindowMode mode_request)
{
  if (getWindowMode() == mode_request)
  {
    return;
  }

  auto* monitor = glfwGetPrimaryMonitor();
  updateMonitorInfo(monitor);
  switch (mode_request)
  {
    case (GameSettings::WindowMode::EXCLUSIVE_FULLSCREEN):
    case (GameSettings::WindowMode::BORDERLESS_FULLSCREEN):
    {
      if(mode_request == GameSettings::WindowMode::EXCLUSIVE_FULLSCREEN)
      {
        auto [width, height] = resolution_info.window;
        glfwSetWindowMonitor(window, monitor, 0, 0, width, height, resolution_info.desktop[2]);
      }
      else
      {
        auto [width, height, refresh] = resolution_info.desktop;
        glfwSetWindowMonitor(window, monitor, 0, 0, width, height, refresh);
      }
      break;
    }

    case (GameSettings::WindowMode::BORDERLESS_WINDOWED):
    {
      auto [width, height] = resolution_info.window;
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
      glfwSetWindowMonitor(window, nullptr, 0, 0, width, height, GLFW_DONT_CARE);
      centerWindow();
      break;
    }

    case (GameSettings::WindowMode::WINDOWED):
    {
      auto [width, height] = resolution_info.window;
      glfwSetWindowMonitor(window, nullptr, 0, 0, width, height, GLFW_DONT_CARE);
      centerWindow();
      break;
    }
  }

  glfwFocusWindow(window);
  windowMode() = mode_request;
}

/**
 * Updates the monitor info.
 * Currently, if no monitor is passed in it will retrieve the
 * primary monitor and read its resolution details including
 * refresh rate.
 *
 * @param [in] monitor The monitor being checked.
 */
void ASGE::GLRenderer::updateMonitorInfo(GLFWmonitor* monitor)
{
  if (monitor != nullptr)
  {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    resolution_info.desktop[0]   = mode->width;
    resolution_info.desktop[1]   = mode->height;
    resolution_info.desktop[2]   = mode->refreshRate;
  }
}

ASGE::Viewport ASGE::GLRenderer::getViewport() const
{
  return resolution_info.viewport;
}

/**
 * Sets the viewport to use for rendering.
 * @param vp The viewport to set.
 */
void ASGE::GLRenderer::setViewport(const ASGE::Viewport& vp)
{
  if(renderTarget() == nullptr)
  {
    if (resolution_policy == Resolution::Policy::MAINTAIN)
    {
      fitViewPort(vp);
      return;
    }

    if (resolution_policy == Resolution::Policy::SCALE)
    {
      fillViewPort(vp);
      return;
    }

    if (resolution_policy == Resolution::Policy::CENTER)
    {
      centerViewPort(vp);
      return;
    }
  }

  resolution_info.viewport = vp;
  saveState();
}

/**
 *  Sets the colour used to clear the screen.
 *  Makes a copy of the clear colour but also signals to the
 *  renderer the colour to use going forward. This prevents
 *  a clear colour call on every update.
 *
 *  @param [in] rgb The clear colour for the buffer.
 */
void ASGE::GLRenderer::setClearColour(ASGE::Colour rgb)
{
  clearColour() = rgb;
  glClearColor(rgb.r, rgb.g, rgb.b, 1.0F);
}

/**
 *  Sets the sprite rendering mode.
 *  Proxies the request to the sprite batch. Allows the use of
 *  different rendering modes, including but not limited to
 *  immediate, deferrred and sorted.
 *
 *  @param [in] mode The sprite sort mode requested.
 *  @see ASGE::SpriteSortMode
 */
void ASGE::GLRenderer::setSpriteMode(SpriteSortMode mode)
{
  batch.setSpriteMode(mode);
}

/**
 *  Sets the title of the window.
 *  @param [in] str The window title.
 */
void ASGE::GLRenderer::setWindowTitle(const char* str)
{
  glfwSetWindowTitle(this->window, str);
}

/**
 *  Returns the active font.
 *  The text renderer will have a font toggled as in use.
 *  This function will return the font.
 *  @return A const reference to the currently active font.
 *  @see ASGE::Font
 */
const ASGE::Font& ASGE::GLRenderer::getDefaultFont() const
{
  return this->text_renderer->getDefaultFont();
}

/**
 *  Sets the active font.
 *  The text renderer will have a font toggled as in use.
 *  This function will set the active font. This will only
 *  work with pre-loaded fonts.
 *  @param [in] The index of the pre-loaded font.
 *  @see ASGE::Font
 */
void ASGE::GLRenderer::setFont(int id)
{
  this->text_renderer->setDefaultFont(id);
}

/**
 *  Loads a font.
 *  Before the text renderer can render in a type face, the font
 *  will need to be loaded. IF successful the font will be assigned
 *  an id which can used by the client to load the new font at
 *  any time.
 *
 *  @param [in] The font to load including its file path.
 *  @param [in] The glyph size to generate the atlas with.
 *  @return The index of the newly loaded font.
 *  @see ASGE::Font
 */
const ASGE::Font* ASGE::GLRenderer::loadFont(const char* font, int size, double range)
{
  return this->text_renderer->loadFont(font, size, range);
}

const ASGE::Font* ASGE::GLRenderer::loadFontFromMem(
  const char* name, const unsigned char* data, unsigned int len, int size, double range)
{
  return this->text_renderer->loadFontFromMem(name, data, len, size, range);
}

const ASGE::Font* ASGE::GLRenderer::loadFontAtlas(
  Font::AtlasMetrics&& metrics, const std::string& img_path, const std::string& csv_path)
{
  return this->text_renderer->loadFontFromAtlas(std::move(metrics), img_path, csv_path);
}

ASGE::SHADER_LIB::Shader* ASGE::GLRenderer::initPixelShader(std::string shader_source)
{
  if (shader_source.empty())
  {
    return nullptr;
  }

  Logging::TRACE("Initialising shader");
  Logging::TRACE(shader_source);

  auto *shader = sprite_renderer->initShader(vs_instancing, shader_source);
  if(shader != nullptr)
  {
  		return shader;
  }

  Logging::ERRORS("Failed to load shader");
  return nullptr;
}

void ASGE::GLRenderer::setActiveShader(ASGE::SHADER_LIB::Shader* shader)
{
  auto *gl_shader = dynamic_cast<ASGE::SHADER_LIB::GLShader*>(shader);
  sprite_renderer->setActiveShader(gl_shader);
}

ASGE::SHADER_LIB::GLShader* ASGE::GLRenderer::getActiveShader()
{
    return sprite_renderer->activeShader();
}

ASGE::SHADER_LIB::Shader* ASGE::GLRenderer::findShader(int shader_handle)
{
  auto shader = std::find(shaders.begin(), shaders.end(), shader_handle);
  if (shader != shaders.end())
  {
    return &(*shader);
  }
  return nullptr;
}

ASGE::Texture2D* ASGE::GLRenderer::createNonCachedTexture(
  int width, int height, ASGE::Texture2D::Format pixel_format, void* data)
{
  return GLTextureCache::getInstance().createNonCached(width, height, pixel_format, data);
}

ASGE::Texture2D* ASGE::GLRenderer::createCachedTexture(
  std::string id, int width, int height, ASGE::Texture2D::Format pixel_format, void* data)
{
  return GLTextureCache::getInstance().createCached(id, width, height, pixel_format, data);
}

ASGE::Texture2D*
ASGE::GLRenderer::createNonCachedMSAATexture(int width, int height, ASGE::Texture2D::Format pixel_format)
{
  return GLTextureCache::getInstance().createNonCachedMSAA(width, height, pixel_format);
}

ASGE::Texture2D* ASGE::GLRenderer::createNonCachedTexture(const std::string& path)
{
  return GLTextureCache::getInstance().createNonCached(path);
}

ASGE::Texture2D* ASGE::GLRenderer::createCachedTexture(const std::string& path)
{
  return GLTextureCache::getInstance().createCached(path);
}

const ASGE::Font& ASGE::GLRenderer::getFont(int idx) const
{
  return this->text_renderer->getFont(idx);
}

int ASGE::GLRenderer::windowHeight() const noexcept
{
  return resolution_info.window[1];
}

int ASGE::GLRenderer::windowWidth() const noexcept
{
  return resolution_info.window[0];
}

/**
 *  Renders a sprite.
 *  The renderer will take a sprite and proxy it on to the batch
 *  where depending on the render mode it will either be rendered
 *  or batched for later.
 *
 *  @param [in] spr The sprite to render.
 */
void ASGE::GLRenderer::render(const Sprite& spr)
{
    batch.renderSprite(spr);
}

void ASGE::GLRenderer::render(const ASGE::Text& text)
{
  this->batch.renderText(text);
}

void ASGE::GLRenderer::render(ASGE::Text&& text)
{
  this->batch.renderText(text);
}

ASGE::SHADER_LIB::Shader* ASGE::GLRenderer::getShader()
{
  return this->findShader(sprite_renderer->getBasicSpriteShaderID());
}

void ASGE::GLRenderer::setProjectionMatrix(float min_x, float max_x, float min_y, float max_y)
{
  ASGE::Camera::CameraView view{ min_x, max_x, min_y, max_y };
  setProjectionMatrix(view);
}

void ASGE::GLRenderer::setProjectionMatrix(const Camera::CameraView& view)
{
  resolution_info.view = view;
  saveState();
}

void ASGE::GLRenderer::setRenderTarget(RenderTarget* render_target)
{
  batch.flush();
  if(auto *asGLRender = dynamic_cast<ASGE::GLRenderTarget*>(render_target); asGLRender != nullptr)
  {
    asGLRender->use();
    active_buffer = render_target;
    return;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  active_buffer = nullptr;
}

void ASGE::GLRenderer::render(const ASGE::Tile& tile, const ASGE::Point2D& xy)
{
  ASGE::GLSprite sprite {tile};
  sprite.xPos(xy.x);
  sprite.yPos(xy.y);
  render(sprite);
}

/**
 * Renders a texture.
 * Instead of using a Sprite or Tile, one can manually render a texture
 * if they so wish. Though the other approaches are preferable.
 * @param texture The texture to render.
 * @param rect The source rectangle to use.
 * @param pos_xy The position of the texture in world space.
 * @param width The resultant width to render.
 * @param height The resultant height to render.
 * @param z_order The z-order to apply.
 */
void ASGE::GLRenderer::render(
  ASGE::Texture2D& texture, std::array<float, 4> rect,
  const Point2D& pos_xy, int width, int height, int16_t z_order)
{
  ASGE::GLSprite sprite;
  sprite.xPos(static_cast<float>(pos_xy.x));
  sprite.yPos(static_cast<float>(pos_xy.y));
  sprite.attach(&texture, Sprite::AttachMode::DEFAULT);
  sprite.width(static_cast<float>(width));
  sprite.height(static_cast<float>(height));
  sprite.setGlobalZOrder(z_order);
  std::copy(rect.begin(), rect.end(), sprite.srcRect());
  render(sprite);
}

/**
 * Attempts to center the game window.
 * Uses the desktop and window resolutions to offset correctly.
 */
void ASGE::GLRenderer::centerWindow()
{
  auto [window_width, window_height] = resolution_info.window;
  auto [desktop_width, desktop_height, desktop_refresh] = resolution_info.desktop;

  glfwSetWindowPos(
    window,
    desktop_width * 0.5 - window_width  * 0.5,
    desktop_height * 0.5 - window_height * 0.5);
}

/**
 * Generates a tuple detailing the desktop's resolution settings.
 * @return Width, Height and Refresh.
 */
std::tuple<int32_t, int32_t, int16_t> ASGE::GLRenderer::screenRes()
{
  return std::make_tuple(resolution_info.desktop[0], resolution_info.desktop[1], resolution_info.desktop[2]);
}

/**
 * Returns the current resolution information.
 * @return Resolutions in use.
 */
const ASGE::Resolution& ASGE::GLRenderer::resolution() const
{
  return resolution_info;
}

/**
 * The base design resolution of the game.
 * When designing the game this resolution is used to control how
 * the screen is mapped when using a different resolution, whether
 * that be smaller or larger. When used correctly sprites positioned
 * on the screen will remain in their relative location (depending
 * on the chosen resolution policy).
 * @param width The width of the game's width.
 * @param height The height of the game's height.
 * @param policy The scaling policy to apply.
 * @see ASGE::Resolution::Policy
 */
void ASGE::GLRenderer::setBaseResolution(int width, int height, ASGE::Resolution::Policy policy)
{
  resolution_info.base[0] = width;
  resolution_info.base[1] = height;
  resolution_policy = policy;
  framebufferSizeCallback(this->window, resolution_info.window[0], resolution_info.window[1]);
}

/**
 * Sets the resolution policy.
 * Calling this will cause the existing viewport to be overwritten
 * with a new one based on the window's width and height.
 * @note If using a custom viewport, it's recommended to set it
 * after this function call, to ensure its size is correct.
 * @param policy The new policy to apply to scaling.
 */
void ASGE::GLRenderer::setResolutionPolicy(ASGE::Resolution::Policy policy)
{
  resolution_policy = policy;
  framebufferSizeCallback(this->window, resolution_info.window[0], resolution_info.window[1]);
}

/**
 * Creates a cached version of a 2D Texture Array
 * 2D Texture Arrays can be used to store images on different layers.
 * @param id The unique id to use.
 * @param width The width of the textures in the array.
 * @param height The height of the textures in the array.
 * @param format The format of the pixels being stored.
 * @param data Initial data to upload.
 * @param count The number of layers to allocate.
 * @return Thr newly allocated 2D texture array.
 */
ASGE::Texture2D* ASGE::GLRenderer::createCachedTextureArray(
  std::string id, int width, int height, ASGE::Texture2D::Format format, void* data, int count)
{
  return GLTextureCache::getInstance().allocateTextureArray(width, height, format, data, count);
}

/**
 * Saves the current View/Projection information.
 * These states are linked to render batches. When a render batch uses a
 * new state, it will apply the settings stored here. This allows batching
 * to work even when the viewport and camera view's are altered during the
 * render stages.
 * @see apply
 */
void ASGE::GLRenderer::saveState()
{
  RenderState state{};
  state.viewport = getViewport();
  constexpr float min = std::numeric_limits<decltype(RenderQuad::z_order)>::min();
  constexpr float max = std::numeric_limits<decltype(RenderQuad::z_order)>::max();
  auto view  = resolution_info.view;
  state.projection = glm::ortho(view.min_x, view.max_x, view.max_y, view.min_y, min, max);
  batch.saveState(std::move(state));
}

/**
 * Returns the current resolution settings.
 * These settings refer to the most recent user requested viewports,
 * views as well as the window and desktop resolutions.
 * @return The renderer's current resolution settings.
 */
const ASGE::Resolution& ASGE::GLRenderer::getResolutionInfo() const
{
  return resolution_info;
}

std::vector<ASGE::SHADER_LIB::GLShader> ASGE::GLRenderer::shaders;