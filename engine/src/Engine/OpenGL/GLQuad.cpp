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

#include "GLQuad.hpp"
#include <utility>

ASGE::GPUQuad::GPUQuad(const ASGE::GPUQuad& rhs) :
  position(rhs.position),
  color(rhs.color),
  uv_data(rhs.uv_data)
{

}

ASGE::GPUQuad& ASGE::GPUQuad::operator=(const ASGE::GPUQuad& rhs)
{
  this->position = rhs.position;
  this->color    = rhs.color;
  this->uv_data  = rhs.uv_data;
  return *this;
}

ASGE::GPUQuad::GPUQuad(ASGE::GPUQuad&& rhs) noexcept  :
  position(std::exchange(rhs.position, glm::mat4{1})),
  color(std::exchange(rhs.color, glm::vec4{1})),
  uv_data(std::move(rhs.uv_data))
{
}


ASGE::RenderQuad::RenderQuad(ASGE::RenderQuad&& rhs) noexcept :
  shader_id(std::exchange(rhs.shader_id, 0)),
  texture_id(std::exchange(rhs.texture_id, 0)),
  z_order(std::exchange(rhs.z_order, 0)),
  gpu_data(std::move(rhs.gpu_data)),
  distance(rhs.distance),
  state(rhs.state)
{

}

ASGE::RenderQuad::RenderQuad(const ASGE::RenderQuad& rhs) :
  shader_id(rhs.shader_id),
  texture_id(rhs.texture_id),
  z_order(rhs.z_order),
  gpu_data(rhs.gpu_data),
  distance(rhs.distance),
  state(rhs.state)
{

}

ASGE::RenderQuad& ASGE::RenderQuad::operator=(const ASGE::RenderQuad& rhs)
{
  this->shader_id  = rhs.shader_id;
  this->texture_id = rhs.texture_id;
  this->z_order    = rhs.z_order;
  this->gpu_data   = rhs.gpu_data;
  this->distance   = rhs.distance;
  this->state      = rhs.state;
  return *this;
}

ASGE::GPUQuad& ASGE::GPUQuad::operator=(ASGE::GPUQuad&& rhs) noexcept
{
  this->position = std::exchange(rhs.position, glm::mat4{1.f});
  this->color    = std::exchange(rhs.color, glm::vec4{1,1,1,1});
  this->uv_data  = rhs.uv_data;
  return *this;
}

ASGE::RenderQuad& ASGE::RenderQuad::operator=(ASGE::RenderQuad&& rhs) noexcept
{
  this->shader_id  = std::exchange(rhs.shader_id, 0);
  this->texture_id = std::exchange(rhs.texture_id, 0);
  this->z_order    = std::exchange(rhs.z_order, 0);
  this->gpu_data   = std::move(rhs.gpu_data);
  this->distance   = rhs.distance;
  this->state      = rhs.state;
  return *this;
}
