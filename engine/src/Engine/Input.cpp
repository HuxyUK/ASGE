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

ASGE::Input::Input()
{
  callback_funcs.reserve(50);
}

ASGE::Input::~Input()
{
  callback_funcs.clear();
}

int ASGE::Input::registerCallback(EventType type, InputFnc fnc)
{
  callback_funcs.push_back(InputFncPair(type, fnc));
  return static_cast<int>(callback_funcs.size() - 1);
}

void ASGE::Input::sendEvent(EventType type, SharedEventData data)
{
  if (use_threads)
  {
    // run callbacks in new thread
    auto clbk = [](InputFncs& fncs, EventType type, SharedEventData& data) {
      for (const auto& callback : fncs)
      {
        if (callback.first == type)
        {
          callback.second(data);
        }
      }
    };

    auto task = std::async([&] { return clbk(callback_funcs, type, data); });
  }
  else
  {
    for (const auto& callback : callback_funcs)
    {
      if (callback.first == type)
      {
        callback.second(data);
      }
    }
  }
}

void ASGE::Input::unregisterCallback(unsigned int id)
{
  if (id > 0 && id < callback_funcs.size())
  {
    callback_funcs.erase(callback_funcs.begin() + id);
  }
}