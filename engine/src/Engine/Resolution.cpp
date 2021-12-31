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

#include "Resolution.hpp"
float ASGE::Resolution::getBaseAspectRatio()
{
  return static_cast<float>(base[0]) / static_cast<float>(base[1]);
}

float ASGE::Resolution::getWindowAspectRatio()
{
  return static_cast<float>(window[0]) / static_cast<float>(window[1]);
}

float ASGE::Resolution::getDesktopAspectRatio()
{
  return static_cast<float>(desktop[0]) / static_cast<float>(desktop[1]);
}
