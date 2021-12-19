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

#include "Font.hpp"
int ASGE::Font::pxWide(const std::string& string) const
{
  return static_cast<int>(pxWide(string, 1.0));
}

int ASGE::Font::pxHeight(const std::string& string) const
{
  return static_cast<int>(pxHeight(string, 1.0));
}

int ASGE::Font::pxWide(const char* ch, float scale) const
{
  return static_cast<int>(pxWide(std::string(ch), scale));
}

int ASGE::Font::pxHeight(const char* ch, float scale) const
{
  return static_cast<int>(pxHeight(std::string(ch), scale));
}

std::tuple<float, float> ASGE::Font::boundsY(const char* ch, float scale) const
{
  return boundsY(std::string(ch), scale);
}
