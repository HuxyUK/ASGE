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

//! @file NonCopyable.hpp
//! @brief Class @ref ASGE::NonCopyable

#pragma once
namespace ASGE
{
	/**
	*  @brief A class that can not be copied.
	*  
	*  Classes that inherit this can not be copied. This is used
	*  to ensure that data isn't lost, spliced or multi-managed
	*  by more than instance.
	*/
	class NonCopyable
	{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;

   public:
		NonCopyable(const NonCopyable &) = delete;
		void operator=(const NonCopyable &) = delete;
	};
}  // namespace ASGE
