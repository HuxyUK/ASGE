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

//! @file FileIO.hpp
//! @brief
//! Namespace @ref ASGE::FILEIO,
//! Struct @ref ASGE::FILEIO::IOBuffer,
//! Struct @ref ASGE::FILEIO::File

#pragma once
#include <memory>
#include <string>
#include <vector>

struct PHYSFS_File;
namespace ASGE
{
  /**
   * @namespace ASGE::FILEIO
   * @brief input and output resource management
   */
  namespace FILEIO
  {
    /**
     * @brief Sets the base directory for saving files.
     *
     * This directory will be used as the root for any files
     * being written to or saved. For obvious reasons, you should
     * look to use a directory different to the gamedata directory.
     * If the folder does not exist or the file subsystem is not
     * initialised then this will fail. The user must have read/write
     * access to the directory for saving to work. It is recommended
     * that the directory used is free of important files.
     *
     * @param dir The root directory for saving files.
     * @return Whether the directory was set successfully or not.
     */
    bool setWriteDir(const std::string& dir, bool mount);

    /*!
      @brief Creates a subdirectory in the root of the write directory.

      Will attempt to create a subdirectory inside the writeable root.
      Ensure that the FileIO subsystem is initialised and that
      a write directory is set (if not using the default).

      @param[in] dir The directory to list.
      @return False if the directory can't be created.
    */
    bool createDir(const std::string& dir);

    /**
     * @brief Prints files stored in a given directory using the ASGE
     * file IO subsystem. This does not list files outside of the
     * virtual mounted paths.
     *
     * @param[in] dir The directory to list.
     */
    void printFiles(std::string&& dir);

    /**
     * @brief Lists all files in a given directory.
     *
     * Generates a list of strings representing each file in a given
     * directory. The search is performed in the pseudo-filesystem
     * and does not conform to the local directories.
     *
     * @param[in] dir The directory to search.
     * @return A vector of strings, one per file found.
     */
    std::vector<std::string> enumerateFiles(const std::string& dir);

    /**
     * @brief Mounts a real directory into the pseudo-fs.
     *
     * Mount a directory in the file system to reside within
     * the pseudo-fs. All mount points exist below the root /data
     *
     * @param[in] dir The directory to mount.
     * @param[in] mount_point The mount point to reside below /data/
     * @return True if successful.
     */
    bool mount(const std::string& dir, const std::string& mount_point);

    /**
     * @brief Attempts to delete a file.
     *
     * Deletes a file using the mounted pseudo-fs. Warning; make
     * sure you specify the correct file for deletion.
     *
     * @param[in] filename File to delete.
     * @return Success.
     */
    bool deleteFile(const std::string& filename);

    /**
     * @brief buffer for raw data.
     *
     * A IOBuffer is a simple structure that stores data
     * as bytes along with the number of bytes in use. This can
     * be used whenever raw data is being written or read from
     * files stored locally.
     */
    struct IOBuffer
    {
      IOBuffer()  = default;
      ~IOBuffer() = default;

      IOBuffer(const IOBuffer& rhs);
      IOBuffer& operator=(const IOBuffer& rhs);

      // TODO: move to std::byte to store data
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
      std::unique_ptr<char[]> data = nullptr; /**< The data buffer.
                                              The bytes that represent the data being stored are
                                              kept in this unique pointer. It will auto deallocate
                                              upon destruction, so do not directly link to the data.
                                              */

      size_t length = 0; /**< Length of buffer.
                         The length of the buffer. This needs to be manually set
                         when appropriate and updated if the data is recreated. */

      // NOLINTNEXTLINE(readability-identifier-naming)
      char* as_char() noexcept; /**< Buffer as char array.
                                Returns the buffer as char array. */

      // NOLINTNEXTLINE(readability-identifier-naming)
      unsigned char* as_unsigned_char() noexcept; /**< Buffer as unsigned char array.
                                                  Returns the buffer as an unsigned char array. */

      // NOLINTNEXTLINE(readability-identifier-naming)
      void* as_void() noexcept; /**< Buffer as void pointer.
                                Returns the buffer as a void pointer. */

      /**
       * @brief Adds data to the buffer.
       *
       * Uses the sizeof operator to determine the number of
       * bytes to reserve when appending the data to the existing
       * buffer. Warning, use of pointers will result in the
       * data failing to be copied correctly. Use of non basic types
       * will require an explicit template instantiation or possibly
       * a specialisation.
       *
       * @tparam[in] T The data to be appended.
       */
      template<typename T>
      void append(T);

      /**
       * @brief Adds data to the buffer.
       *
       * Copies n_bytes of data to the back of the buffer.
       *
       * @param[in] data The memory location to copy from.
       * @param[in] bytes The number of bytes to copy.
       */
      void append(const char* data, size_t bytes);

      /**
       * @brief Clears the data buffer.
       *
       * Useful when wanting to reuse an existing buffer, or
       * to ensure the buffer is empty before adding new data.
       */
      void clear() noexcept;
    };

    /**
     * @brief A file stored locally on the machine.
     *
     * A file can be used to open a resource. It supports reading
     * and writing and provides an abstraction between the low-level
     * IO libraries. Reading from a file will generate an IOBuffer
     * object, which can be used to load resources such as fonts.
     */
    struct File
    {
     public:

      /**
       * The mode a file is accessed in.
       */
      enum class IOMode
      {
        READ,   /**< The file is open in READ only mode. */
        APPEND, /**< The file is open in APPEND mode.    */
        WRITE   /**< The file is open in WRITE mode.     */
      };

     public:
      File() = default;
      virtual ~File();
      File(const File& rhs) = delete;
      File& operator=(const File& rhs) = delete;

      /**
       * @brief Attempts to open a local file.
       *
       * The low-level library will attempt to open the file using the specified
       * IOMode.
       *
       * @param filename The name of the file to open.
       * @param mode The IO mode required.
       * @return True if successful.
       */
      bool open(const std::string& filename, IOMode mode = IOMode::READ);

      /**
       * @brief Closes the file (if open).
       *
       * Clears the handle pointing to the file/resource if open.
       * @return True if successful.
       */
      bool close() noexcept;

      /**
       * @brief Seeks to a new position.
       *
       * Increments the read position of the file by a user specified
       * number of bytes.
       *
       * @param bytes The distance to seek (in bytes).
       * @return True if successful.
       */
      bool seek(std::size_t bytes) noexcept;

      /**
       * @brief Writes an IOBuffer into the file.
       *
       * Using an IOBuffer, it's contents will be directly appended.
       *
       * @param buffer The data to be written.
       * @return The number of bytes written.
       */
      long long int write(IOBuffer& buffer) noexcept;

      /**
       * @brief Reads the contents of the file.
       *
       * Stores the entire contents of the open file into a buffer.
       * The buffer stores both the data and its length.
       *
       * @return The data buffer.
       * @see IOBuffer
       */
      IOBuffer read();

     private:
      PHYSFS_File* handle = nullptr; /**< The low-level handle to the file. */
    };

  } // namespace FILEIO
} // namespace ASGE
