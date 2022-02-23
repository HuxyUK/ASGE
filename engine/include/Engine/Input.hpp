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

/**
 * @file
 * @brief Class @ref ASGE::Input
 */

#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include "Gamepad.hpp"
#include "InputEvents.hpp"
#include "Mouse.hpp"

namespace ASGE {
	class Renderer;

	/**
	 *  @brief The input handler system.
   *
   *  The input class is designed to work alongside the renderer and is
   *  responsible for sending events to interested parties. It will
   *  also provide helper functions that can retrieve non-event driven
   *  data such as the cursor position or the gamepad status. Much of
   *  the underlying implementation will be platform specific, with
   *  input offering a compatible interface.
	 *
	 * <example>
	 * @code
	 * // register a key callback
	 * key_callback_id = this->inputs->addCallbackFnc(ASGE::EventType::E_KEY, &SampleGame::input, this);
	 *
	 * // input handler for keys
	 * void SampleGame::input(const ASGE::SharedEventData data)
	 * {
	 *   const auto *key_event = dynamic_cast<const ASGE::KeyEvent*>(data.get());
	 *   auto action = key_event->action;
	 *   auto key = key_event->key;
	 *   if (key == ASGE::KEYS::KEY_ESCAPE)
	 *   {
	 *     signalExit();
	 *   }
	 * }
	 *
	 * // unregister a key callback
	 * this->inputs->unregisterCallback(key_callback_id);
	 * @endcode
	 * </example>
	 */
	class Input
	{

	public:

   using CallbackID = std::string;

		/**
		* Default Constructor.
		*/
		Input();
    Input(const Input& rhs) = delete;
    Input& operator=(const Input& rhs) = delete;

		/**
		* Destructor.
		* Clears all callback functions.
		*/
		virtual ~Input();
		
		/**
		* Destructor.
		* Initialises and sets up the input system.
		* @param renderer A pointer to the linked renderer system. 
		* @return Returns whether the input system initialised.
		*/
		virtual bool init(Renderer* renderer) = 0;
		
		/**
		* Updates the input handler.
		*/
		virtual void update() = 0;

    /**
     * Updates the GamePad mappings used.
     * The engine makes use of a third party solution to correctly
     * map button presses to connected gamepads in a consistent
     * way, regardless of manufacturer or type. The aims is to solve
     * the problem with the "X" button differs depending on the model
     * of gamepad used. The file format should follow the project
     * SDL_GameControllerDB which is the mapping system uses.
     * @param[in] mappings_file The updated GamePad mappings.
     * @see https://github.com/gabomdq/SDL_GameControllerDB
     */
    virtual void updateGamePadMappings(const std::filesystem::path &mappings_file) = 0;

		/**
		* Gets the cursor's (mouse) position.
		* Grabs the current position in screen space of the mouse
		* cursor. This can be used to detect the location of clicks
		* as well as its current position. The positions are stored
		* directly in the two parameters passed in. 
		* @param[in] xpos The position in the X axis.
		* @param[in] ypos The position in the Y axis.
		*/
		virtual void getCursorPos(double &xpos, double &ypos) const = 0;

		/**
		* Sets the cursor's (mouse) mode.
		* Allows control over the mouse cursors visibility and 
		* whether or not it should be locked to the window. 
		* @param[in] mode The cursor mode to set.
		* @see ASGE::CursorMode
		*/
		virtual void setCursorMode(ASGE::MOUSE::CursorMode mode) = 0;

		/// Sets the cursor's current position
		/// \param[in] x The x position to move the mouse to.
		/// \param[in] y The y position to move the mouse to.
		virtual void setCursorPos(double x, double y) const = 0;

		/**
		* Obtains the controllers data.
		* Searches for a controller connected at a specific idx and
		* returns any data relating to it, including naming, axis 
		* and button states. This is currently polled (demanded) rather
		* than event driven and is normally handled by the renderer's
		* window. 
		* @param[in] idx The id of the connected controller.
		* @return The game controller's connected state and data.
		* @see GamePadData
		*/
		[[nodiscard]] virtual GamePadData getGamePad(int idx) const = 0;

    /**
    * Obtains the first connected controllers data.
    * Searches for the first connected controller and then returns
		* any data relating to it, including naming, axis and button states.
    * This is currently polled (demanded) rather than event driven and
    * is normally handled by the renderer's window.
		* @return The first connected gamepad and its data.
		* @see GamePadData
    */
		[[nodiscard]] virtual GamePadData getGamePad() const = 0;

    /**
    * @brief Retrieves a list of all connected game pads.
    * Searches for all possible connected gamepads and retrieves the data
    * for each one. If no attached gamepads are present then the vector
    * will be empty. This can be used to easily process all connected
    * inputs in a single call, rather than individually checking for each
    * index to see if a gamepad is connected.
    *
    * @return The game controllers connected state and data.

    * <example>
    * Example:
    * @code
    * auto gamepads = inputs->getGamePads();
    * for (const auto& gamepad : gamepads)
    * {
    *   Logging::INFO(gamepad.name);
    * }
    * @endcode
    *
    * @see GamePadData
    */
    [[nodiscard]] virtual std::vector<GamePadData> getGamePads() const = 0;

		/**
		* Sends an event.
		* When an event happens, the type of event and the data
		* relating to it is forwarded to any interested parties
		* via their function pointer. NOTE: the data is shared
		* because the events can be sent using threads and this
		* ensures their lifespan. 
		* @param[in] type The type of event registered.
		* @param[in] data The data relating to the event.
		* @see EventType
		* @see SharedEventData
		*/
		void sendEvent(EventType type, SharedEventData data);

		/**
		* Adds a callback function.
		* Many events are callback driven. This function
		* allows the use of a member function to be called
		* when the event happens. 
		* @param[in] type The type of event being listened for.
		* @param[in] fncPtr The function pointer.
		* @param[in] obj The object (this ptr) the function belongs to.
		* @see EventType
		* @return The handle for the registered callback.
		*/
		template<typename T, typename T2>
    CallbackID addCallbackFnc(EventType type, T fncPtr, T2* obj)
		{
			using namespace std::placeholders;

      //NOLINTNEXTLINE(modernize-avoid-bind)
			return this->registerCallback(type, std::bind(fncPtr, obj, _1));
		}

		/**
		* Adds a callback function.
		* Many events are callback driven. This function
		* allows the use of a function to be called
		* when the event happens.
		* @param[in] type The type of event being listened for.
		* @param[in] fncPtr The function pointer.
		* @see EventType
		* @return the handle for the registered callback.
		*/
		template<typename T>
    CallbackID addCallbackFnc(EventType type, T fncPtr)
		{
			using namespace std::placeholders;
			return this->registerCallback(type, std::bind(fncPtr, _1));
		}

		/**
		 * @brief Removes a callback function.
		 *
		 * This requires a valid handle to work correctly.
		 * The handle is returned when the function event is
		 * added. This prevents callbacks happening for classes
		 * or functions that no longer exist.
		 *
		 * @param[in] id The handle for the registered callback.
		 */
		void unregisterCallback(CallbackID id);

		/**
		* Allows events to use threads. 
		* Rather than sending events and waiting for their 
		* responses, threads can be used to perform the callbacks
		* asynchronously. When using threads, care must be taken
		* to ensure any operations in the callbacks is thread safe. 
		*/
		//NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
		bool use_threads = false;

	private:
    using InputFnc = std::function<void(SharedEventData)>;
    using InputFncPair = std::tuple<EventType, InputFnc>;
    using InputFncs = std::unordered_map<std::string, InputFncPair>;

    /**
     * Registers a callback function with an event.
     * @param eventType The type of event to subscribe too.
     * @param inputFnc The function to call when event happens.
     * @return The handler to the registered event.
     */
		std::string registerCallback(EventType eventType, InputFnc inputFnc);
		InputFncs callback_funcs; //!< The registered callbacks.
	};
}  // namespace ASGE


