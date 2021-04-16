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

#include <Engine/Camera.hpp>
ASGE::Camera::Camera(float width, float height) :
  position{ 0, 0 },
  dimensions{ width, height } {}

ASGE::Camera::Camera(ASGE::Point2D focal_point, float width, float height) :
  position{ focal_point.x, focal_point.y },
  dimensions{ width, height } {}

void ASGE::Camera::update(const ASGE::GameTime& /*game_time*/)
{

}

void ASGE::Camera::lookAt(ASGE::Point2D pos)
{
  this->position.x = pos.x;
  this->position.y = pos.y;
}

ASGE::Camera::CameraView ASGE::Camera::getView() const
{
  ASGE::Camera::CameraView view;
  view.min_x = (position.x * zoom) - dimensions[0] * 0.5F;
  view.min_y = (position.y * zoom) - dimensions[1] * 0.5F;
  view.max_x = view.min_x + dimensions[0] * zoom;
  view.max_y = view.min_y + dimensions[1] * zoom;
  return view;
}

void ASGE::Camera::translate(float x, float y, float z)
{
  position.x += x;
  position.y += y;
  zoom += z;
}

void ASGE::Camera::translateY(float y)
{
  position.y += y;
}

void ASGE::Camera::translateZ(float z)
{
  zoom += z;
}

void ASGE::Camera::translateX(float x)
{
  position.x += x;
}

void ASGE::Camera::resize(float width, float height)
{
  dimensions[0] = width;
  dimensions[1] = height;
}

float ASGE::Camera::getZoom() const
{
  return zoom;
}

void ASGE::Camera::setZoom(float z)
{
  this->zoom = z;
}
