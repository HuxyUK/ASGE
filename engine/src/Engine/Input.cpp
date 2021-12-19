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

#include "Input.hpp"
#include <future>
#include <uuid.h>


uuids::uuid_random_generator uuidGenerator() {
  std::random_device rd;
  auto seed_data = std::array<int, std::mt19937::state_size>{};
  std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
  std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
  static std::mt19937 engine(seq);
  static uuids::uuid_random_generator uuid_random_generator(&engine);
  return uuid_random_generator;
}

ASGE::Input::Input()
{
  callback_funcs.reserve(50);
}

ASGE::Input::~Input()
{
  callback_funcs.clear();
}

ASGE::Input::CallbackID ASGE::Input::registerCallback(EventType type, InputFnc fnc)
{
  static auto uuid_gen = uuidGenerator();
  auto uuid = uuid_gen();
  callback_funcs[uuids::to_string(uuid)] = InputFncPair(type, fnc);
  return to_string(uuid);
}

void ASGE::Input::sendEvent(EventType type, SharedEventData data)
{
  auto send = [](InputFncs& callbacks, EventType type, SharedEventData& data) {
    for (const auto &[uuid, callback] : callbacks)
    {
      const auto&[callback_type, callback_function] = callback;
      if (callback_type == type)
      {
        callback_function(data);
      }
    }
  };

  if (use_threads)
  {
    auto task = std::async([&] { return send(callback_funcs, type, data); });
  }
  else
  {
    send(callback_funcs, type, data);
  }
}

void ASGE::Input::unregisterCallback(CallbackID id)
{
  callback_funcs.erase(id);
}
