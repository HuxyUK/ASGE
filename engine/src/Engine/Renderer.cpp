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

#include <Engine/FileIO.hpp>
#include <Engine/Renderer.hpp>
#include <filesystem>
#include <fstream>

ASGE::GameSettings::WindowMode ASGE::Renderer::getWindowMode() noexcept
{
	return this->window_mode;
}

ASGE::SHADER_LIB::Shader* ASGE::Renderer::initPixelShaderFromFile(const std::string& path)
{
  const std::filesystem::path FS_PATH(path);
  if (std::filesystem::exists(FS_PATH))
  {
    auto file = std::ifstream(FS_PATH.c_str());
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return initPixelShader(std::string(str));
  }

  using namespace ASGE::FILEIO;
	auto file = File();
	if(!file.open(path))
	{
		return nullptr;
	}

	auto buffer = file.read();
	if(!buffer.data || (buffer.length == 0U))
	{
		return nullptr;
	}

	return initPixelShader(std::string(buffer.data.get(), buffer.length));
}

void ASGE::Renderer::renderText(std::string str, int x, int y)
{
  renderText({getDefaultFont(), std::move(str), x, y, default_text_colour});
}

void ASGE::Renderer::renderText(std::string str, int x, int y, const Colour& colour)
{
  renderText({getDefaultFont(), std::move(str), x, y, colour});
}

void ASGE::Renderer::render(const ASGE::Sprite& sprite)
{
  renderSprite(sprite);
}

void ASGE::Renderer::render(const ASGE::Text& text)
{
  renderText(text);
}

void ASGE::Renderer::render(const ASGE::Text&& text)
{
  renderText(text);
}