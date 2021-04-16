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

#include "GLRenderer.hpp"
#include "GLAtlasManager.h"
#include "GLFontSet.hpp"
#include "GLInput.hpp"
#include "GLLegacySpriteRenderer.hpp"
#include "GLModernSpriteRenderer.hpp"
#include "GLRenderTarget.hpp"
#include "GLSprite.hpp"
#include "GLTextureCache.hpp"
#include <Engine/Camera.hpp>
#include <OpenGL/Shaders/GLShaders.vs>
#include <memory>

namespace ASGE
{
  namespace
  {
    struct SHADER_DATA
    {
      glm::mat4 projection;
    } global_shader_data;
  }
}

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
ASGE::GLRenderer::GLRenderer() : Renderer(), batch()
{
  shaders.reserve(20);
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
bool ASGE::GLRenderer::init()
{
  // Window size
  target_width  = ASGE::SETTINGS.window_width;
  target_height = ASGE::SETTINGS.window_height;

  // Init GLFW
  if (glfwInit() == GLFW_FALSE)
  {
    return false;
  }

 // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_SAMPLES, ASGE::SETTINGS.msaa_level);

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

    window = glfwCreateWindow(target_width, target_height, "ASGE", nullptr, nullptr);
    if (window != nullptr)
    {
      Logging::INFO(
        "Launching Window with GLVERSION: " +
        std::to_string(item.first.first) + "." +
        std::to_string(item.first.second) + " Support");

      glfwMakeContextCurrent(window);

      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      {
        Logging::ERRORS("Failed to initialise GLAD");
        throw std::runtime_error("gladLoadGLLoader failed");
        return false;
      }

      updateMonitorInfo(glfwGetPrimaryMonitor());
      centerWindow();
      setWindowedMode(ASGE::SETTINGS.mode);
      glfwShowWindow(this->window);
      item.second();
      RENDER_LIB = sprite_renderer->getRenderLib();
      break;
    }
  }

  setProjectionMatrix(0, 0, static_cast<float>(target_width), static_cast<float>(target_height));

  text_renderer = std::make_unique<GLAtlasManager>();
  text_renderer->init();
  sprite_renderer->init();
  batch.sprite_renderer = sprite_renderer.get();

  switch(SETTINGS.vsync)
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
  return true;
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

const glm::mat4& ASGE::GLRenderer::getProjectionMatrix() const
{
  return projection_matrix;
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
void ASGE::GLRenderer::renderDebug()
{
  batch.flush();

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

  Text debug_text = { getFont(0), debug_string.c_str(), 0, 52, ASGE::COLOURS::PINK };
  batch.renderText(std::move(debug_text));
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
    return input;
  }

  return nullptr;
}

/**
 *  Renders a sprite.
 *  The renderer will take a sprite and proxy it on to the batch
 *  where depending on the render mode it will either be rendered
 *  or batched for later.
 *
 *  @param [in] spr The sprite to render.
 *  @param [in] z_order The order used when sorting draw calls.
 */
void ASGE::GLRenderer::renderSprite(const Sprite& spr)
{
  batch.renderSprite(spr);
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
    case (GameSettings::WindowMode::FULLSCREEN):
    {
      glfwSetWindowMonitor(window, monitor, 0, 0, target_width, target_height, desktop_refresh);
      glViewport(0, 0, target_width, target_height);
      break;
    }

    case (GameSettings::WindowMode::BORDERLESS_FULLSCREEN):
    {
      glfwSetWindowMonitor(window, monitor, 0, 0, desktop_res[0], desktop_res[1], desktop_refresh);

      double width  = 0;
      double height = 0;

      // if aspect ratio isn't bigger..
      if (desktop_res[0] / desktop_res[1] <= target_width / target_height)
      {
        width  = desktop_res[0];
        height = desktop_res[0] *
                 (static_cast<double>(target_height) / static_cast<double>(target_width));
      }
      else
      {
        if (desktop_res[0] >= desktop_res[1])
        {
          height  = desktop_res[1];
          width = desktop_res[1] *
                   (static_cast<double>(target_width) / static_cast<double>(target_height));
        }
        else
        {
          height = desktop_res[1];
          width  = desktop_res[1] *
                  (static_cast<double>(target_width) / static_cast<double>(target_height));
        }
      }

      auto offset_x = (desktop_res[0] - width)  * 0.5;
      auto offset_y = (desktop_res[1] - height) * 0.5;
      glViewport(offset_x, offset_y, width, height);
      break;
    }

    case (GameSettings::WindowMode::BORDERLESS_WINDOWED):
    {
      // not working..
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
      glfwSetWindowMonitor(window, nullptr, 0, 0, target_width, target_height, GLFW_DONT_CARE);
      centerWindow();
      glViewport(0, 0, target_width, target_height);
      break;
    }

    case (GameSettings::WindowMode::WINDOWED):
    {
      glfwSetWindowMonitor(window, nullptr, 0, 0, target_width, target_height, GLFW_DONT_CARE);
      centerWindow();
      glViewport(0, 0, target_width, target_height);
      break;
    }
  }

  glfwFocusWindow(window);
  windowMode() = mode_request;
}

void ASGE::GLRenderer::updateMonitorInfo(GLFWmonitor* monitor)
{
  if (monitor != nullptr)
  {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    desktop_res[0]          = mode->width;
    desktop_res[1]          = mode->height;
    desktop_refresh         = mode->refreshRate;
  }
}

ASGE::Viewport ASGE::GLRenderer::getViewport() const
{
  ASGE::Viewport viewport;
  glGetIntegerv(GL_VIEWPORT, &viewport.x);
  return viewport;
}

void ASGE::GLRenderer::setViewport(const ASGE::Viewport& viewport)
{
  this->batch.flush();
  glViewport(static_cast<int>(viewport.x), static_cast<int>(viewport.y),
             static_cast<int>(viewport.w), static_cast<int>(viewport.h));
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
 *  @param [in] The font size to generate the atlas with.
 *  @return The index of the newly loaded font.
 *  @see ASGE::Font
 */
int ASGE::GLRenderer::loadFont(const char* font, int pt)
{
  return this->text_renderer->loadFont(font, pt);
}

int ASGE::GLRenderer::loadFontFromMem(
  const char* name, const unsigned char* data, unsigned int size, int pt)
{
  return this->text_renderer->loadFontFromMem(name, data, size, pt);
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
  if (gl_shader != nullptr)
  {
    sprite_renderer->setActiveShader(gl_shader);
  }
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

float ASGE::GLRenderer::windowHeight() const noexcept
{
  return target_height;
}

float ASGE::GLRenderer::windowWidth() const noexcept
{
  return target_width;
}

void ASGE::GLRenderer::renderText(const ASGE::Text& text)
{
  this->batch.renderText(text);
}

void ASGE::GLRenderer::renderText(ASGE::Text&& text)
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
  batch.flush();

  float min = std::numeric_limits<decltype(RenderQuad::z_order)>::min();
  float max = std::numeric_limits<decltype(RenderQuad::z_order)>::max();

  projection_matrix = glm::ortho<float>(view.min_x, view.max_x, view.max_y, view.min_y, min, max);
  global_shader_data.projection = projection_matrix;

  if(projection_ubo == -1)
  {
    glGenBuffers(1, &projection_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, projection_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(global_shader_data), &global_shader_data, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, GLRenderConstants::PROJECTION_UBO_BIND, projection_ubo);
  }
  else
  {
    glBindBuffer(GL_UNIFORM_BUFFER, projection_ubo);
  }

  glBufferSubData(
    GL_UNIFORM_BUFFER,
    0,
    sizeof(glm::mat4),
    glm::value_ptr(global_shader_data.projection));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ASGE::GLRenderer::setRenderTarget(const ASGE::RenderTarget* render_target)
{
  batch.flush();
  if(const auto *asGLRender = dynamic_cast<const ASGE::GLRenderTarget*>(render_target); asGLRender != nullptr)
  {
    asGLRender->use();
    return;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ASGE::GLRenderer::render(ASGE::Texture2D& texture, int x, int y)
{
  ASGE::GLSprite sprite;
  sprite.xPos(static_cast<float>(x));
  sprite.yPos(static_cast<float>(y));
  sprite.attach(&texture);
  sprite.width(texture.getWidth());
  sprite.height(texture.getHeight());
  renderSprite(sprite);
}

void ASGE::GLRenderer::centerWindow()
{
  glfwSetWindowPos(
    window,
    desktop_res[0] * 0.5 - target_width  * 0.5,
    desktop_res[1] * 0.5 - target_height * 0.5);
}

std::vector<ASGE::SHADER_LIB::GLShader> ASGE::GLRenderer::shaders;