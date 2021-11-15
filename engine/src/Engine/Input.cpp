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

#include "Engine/Input.hpp"
#include <future>

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
: UUIDGen(uuidGenerator())
{
  callback_funcs.reserve(50);
}

ASGE::Input::~Input()
{
  callback_funcs.clear();
}

ASGE::Input::CallbackID ASGE::Input::registerCallback(EventType type, InputFnc fnc)
{
  auto uuid = UUIDGen();
  callback_funcs[uuid] = InputFncPair(type, fnc);
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
  auto uuid = uuids::uuid::from_string(id).value();
  callback_funcs.erase(uuid);
}