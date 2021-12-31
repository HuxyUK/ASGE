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

//! @file OGLGame.hpp
//! @brief Class @ref ASGE::OGLGame

#pragma once
#include "Game.hpp"

namespace ASGE {
	/**
	*  @brief an OpenGL implementation of the Game engine.
	*  
	*  Supporting modern OpenGL this class allows the update and
	*  rendering of game objects, sprites and textures. It makes 
	*  use of simple shaders and manages the creation and destruction
	*  of the OpenGL window subsystem. Currently it does not support
	*  resizing of windows on the fly, or focus loss. 
	*/
	class OGLGame: public Game
	{
	public:
	  explicit OGLGame(const ASGE::GameSettings& settings);
		~OGLGame() override = default;

		/**
		* Initialises the OpenGL subsystem.
		* @return True if successful.
		*/
		bool initAPI(const ASGE::GameSettings& settings) final;
		
		/**
		* Terminates the OpenGL subsystem.
		* @return True if managed gracefully.
		*/
		bool exitAPI() noexcept final;
		
		/**
		* Prepares the OpenGL subsystem for the next frame.
		*/
		void beginFrame() final;
		
		/**
		* Clean-up the OpenGL subsystem at the end of the frame.
		* This also includes the updating and rendering of FPS,
		* dynamic_batch rendering, queue management and swapping of
		* buffers. 
		*/
		void endFrame() final;

	 private:
    void playSplash();
  };
}  // namespace ASGE
