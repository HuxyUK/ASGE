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

#include <PhysFS/PhysFS.hpp>
#include <cstring>
#include <string>
#include "FileIO.hpp"
#include "Logger.hpp"

using namespace PhysFS;
using namespace ASGE;

using ASGE::FILEIO::File;
using ASGE::FILEIO::IOBuffer;

IOBuffer::IOBuffer(const IOBuffer& rhs) : length(rhs.length)
{
  data = std::make_unique<char[]>(rhs.length);
  memcpy(data.get(), rhs.data.get(), rhs.length);
}

IOBuffer& IOBuffer::operator=(const IOBuffer& rhs)
{
  if (this == &rhs)
  {
    return *this;
  }

  this->length = rhs.length;
  this->data   = std::make_unique<char[]>(rhs.length);
  memcpy(data.get(), rhs.data.get(), rhs.length);
  return *this;
}

void IOBuffer::clear() noexcept
{
  data.reset();
  length = 0;
}

void IOBuffer::append(const char* additional_data, size_t bytes)
{
  std::unique_ptr<char[]> new_buffer = std::make_unique<char[]>(this->length + bytes);
  memcpy(new_buffer.get(), data.get(), length);
  memcpy(new_buffer.get() + length, additional_data, bytes);
  data = std::move(new_buffer);
  this->length += bytes;
}

template<typename T>
void IOBuffer::append(T data_to_append)
{
  const char* as_char = reinterpret_cast<const char*>(&data_to_append);
  append(as_char, sizeof(T));
}

template<>
void IOBuffer::append<std::string>(std::string str)
{
  append(str.data(), str.size());
}

template void IOBuffer::append<bool>(bool);
template void IOBuffer::append<char>(char);
template void IOBuffer::append<int>(int);
template void IOBuffer::append<long>(long);
template void IOBuffer::append<long long>(long long);
template void IOBuffer::append<float>(float);
template void IOBuffer::append<double>(double);

sint64 File::write(IOBuffer& buffer) noexcept
{
  return PhysFS::writeBytes(handle, buffer.as_void(), buffer.length);
}

IOBuffer File::read()
{
  IOBuffer buffer;
  if (handle != nullptr)
  {
    const auto BYTES = static_cast<size_t>(PhysFS::length(handle));
    buffer.data      = std::make_unique<char[]>(BYTES);

    auto bytes_read = PhysFS::readBytes(handle, buffer.as_void(), BYTES);
    buffer.length   = static_cast<size_t>(bytes_read);
  }

  return buffer;
}

bool File::seek(std::size_t bytes) noexcept
{
  return PhysFS::seek(handle, bytes) != 0;
}

char* IOBuffer::as_char() noexcept
{
  return data.get();
}

unsigned char* IOBuffer::as_unsigned_char() noexcept
{
  return reinterpret_cast<unsigned char*>(data.get());
}

void* IOBuffer::as_void() noexcept
{
  return data.get();
}

File::~File()
{
  close();
}

bool File::open(const std::string& filename, IOMode mode)
{
  if (!close())
  {
    return false;
  }

  auto mode_lookup = [](IOMode mode) {
    switch (mode)
    {
      case IOMode::READ:
        return PhysFS::IOMode::READ;
      case IOMode::WRITE:
        return PhysFS::IOMode::WRITE;
      case IOMode::APPEND:
        return PhysFS::IOMode::APPEND;
    }

    return PhysFS::IOMode::READ;
  };

  handle = PhysFS::open(filename, mode_lookup(mode));
  if (handle == nullptr)
  {
    Logging::WARN("ASGE::FILEIO::Failed to open: \"" + filename + "\"");
  }

  return handle != nullptr;
}

bool File::close() noexcept
{
  return PhysFS::close(handle);
}

bool ASGE::FILEIO::setWriteDir(const std::string& dir, bool mount = false)
{
  if (PhysFS::isInititalised())
  {
    PhysFS::unmount(PhysFS::getWriteDir());
    PhysFS::setWriteDir(dir);
    if (mount)
    {
      PhysFS::mount(PhysFS::getWriteDir(), "data", true);
      return true;
    }
  }

  return false;
}

bool ASGE::FILEIO::createDir(const std::string& dir)
{
  if (PhysFS::isInititalised())
  {
    if (PhysFS::mkdir(dir) == PhysFS::IOResult::PHYSFS_OK)
    {
      return true;
    }
  }

  return false;
}

void ASGE::FILEIO::printFiles(std::string&& dir)
{
  if (PhysFS::isInititalised())
  {
    auto files = enumerateFiles(dir);
    for (const auto& file : files)
    {
      auto file_name = std::string(dir).append("/").append(file);
      Logging::INFO(file_name);
      printFiles(std::move(file_name));
    }
  }
}

std::vector<std::string> ASGE::FILEIO::enumerateFiles(const std::string& dir)
{
  if (PhysFS::isInititalised())
  {
    return PhysFS::enumerateFiles(dir);
  }

  return {};
}

bool FILEIO::mount(const std::string& dir, const std::string& mount_point)
{
  if (PhysFS::isInititalised())
  {
    if (PhysFS::mount(dir, "/data/" + mount_point, true) == PhysFS::IOResult::PHYSFS_OK)
    {
      return true;
    }
  }

  return false;
}

bool FILEIO::deleteFile(const std::string& file)
{
  if (PhysFS::isInititalised())
  {
    if (PhysFS::deleteFile(file) == PhysFS::IOResult::PHYSFS_OK)
    {
      return true;
    }
  }
  return false;
}
