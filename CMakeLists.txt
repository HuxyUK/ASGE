cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OSX deployment version")
project(ASGE)

option(ASGE_BUILD_DEMOS     "Build sample demos for ASGE" false)
option(ASGE_ENABLE_DOXYGEN  "Enables doxygen support for ASGE" true)
option(ENGINE_SHARED_LIB    "Build game engine as a shared library" false)
option(FREETYPE_SHARED_LIB  "Build freetype as a shared library" false)
option(GLAD_SHARED_LIB      "Build glad as a shared library" false)
option(GLFW_SHARED_LIB      "Build glfw as a shared libary" false)
option(PHYSFS_SHARED_LIB    "Build physfs++ as a shared library" false)
option(MONOLITHIC_BUILD     "Link all libraries into single dynamic one" true)
option(ENABLE_MSDFGEN       "Build MSDF library for signed distanced fonts" true)

if(ENGINE_SHARED_LIB)
    if(MONOLITHIC_BUILD)
        message("PERFORMING MONOLITHIC BUILD")
        set(GLAD_SHARED_LIB OFF)
        set(GLFW_SHARED_LIB OFF)
        set(PHYSFS_SHARED_LIB OFF)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    else()
        set(BUILD_SHARED_LIBS ON)
        set(ENGINE_SHARED_LIB ON)
    endif()
else()
    set(BUILD_SHARED_LIBS OFF)
endif()

## some additional settings and libs
set(ENABLE_PIE ON)
set(ENABLE_MAGIC_ENUM ON )

# stored libs in /lib and binary files in /bin
# bin
#  |_____ my_executable
#  |_____ mylib.dll
#  |_____ gamedata
#  |
# lib
#  |_____ libmylib.dll.a
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

## all the libs are going here, so let's be lazy about it
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

## shared libs need hints on where to find the libs
if(BUILD_SHARED_LIBS)
    if (APPLE)
        set(CMAKE_MACOSX_RPATH 1)
        set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
    else()
        set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
    endif()
endif()

## useful for debugging builds
set(CMAKE_VERBOSE_MAKEFILE on)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

## enable sanitizers in debug
if ("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
    set(ADDRESS_SANITIZER ON)
    set(UB_SANITIZER ON)
    set(THREAD_SANITIZER ON)
    set(MEMORY_SANITIZER ON)
endif()

## add cmake build scripts
add_subdirectory(cmake)

## build the game engine
add_subdirectory(engine)

## build the examples/game samplers
if(ASGE_BUILD_DEMOS)
    add_subdirectory(examples)
endif(ASGE_BUILD_DEMOS)

## doxygen build target
if(ASGE_ENABLE_DOXYGEN)
    add_custom_target(doxygen COMMAND doxygen WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif(ASGE_ENABLE_DOXYGEN)
