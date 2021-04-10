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

//! @file Value.hpp
//! @brief Namespace @ref ASGE, Class ASGE::ValueBase, Class ASGE::Value< T >
//! Stores the generic value class

#pragma once
#include <any>
namespace ASGE
{
  /**
   * @brief A thin interface for an object of any type.
   */
  class ValueBase
  {
   public:
    ValueBase()          = default;
    virtual ~ValueBase() = default;

    /**
     * Replaces the object being stored being stored.
     * @param object The replacement object to store.
     */
    virtual void set(std::any object) = 0;

    /**
     * Retrieves the value as a pointer wrapped in std::any container.
     * @return A pointer to the object stored.
     */
    virtual std::any get() = 0;
  };

  /**
   * @brief The concrete implementation of the ValueBase class.
   *
   * The value class uses std::any to store an object of any type. By using
   * this templated version the data can be stored internally and access to
   * it is controlled safely via the std::any wrapper. If an incorrect type is
   * used an exception is thrown.
   *
   * @tparam T The type of object to store.
   */
  template<typename T>
  class Value : public ValueBase
  {
   public:
    Value()           = default;
    ~Value() override = default;

    std::any get() override { return &data; }
    void set(std::any object) override { data = std::any_cast<T>(object); }

    /**
     * Retrieves the data stored in the wrapper as a pointer.
     * @return A pointer to the generic value.
     */
    T* value() { return &data; }

   private:
    T data;
  };
} // namespace ASGE
