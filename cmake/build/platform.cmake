#------------------------------------------------------------------------------
# Platform specific variables.
# Small script designed to set platform specific variables to make it easier
# for performing platform specific settings.
#------------------------------------------------------------------------------
message(STATUS "PLATFORM CHECKS")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

if(UNIX AND APPLE)
  set(PLATFORM "OSX" CACHE STRING "" FORCE)
  mark_as_advanced(PLATFORM)
elseif(UNIX)
  set(PLATFORM "LINUX" CACHE STRING "" FORCE)
  mark_as_advanced(PLATFORM)
else()
  set(PLATFORM "WIN32" CACHE STRING "" FORCE)
  mark_as_advanced(PLATFORM)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND "x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC")
    set (PLATFORM "${PLATFORM} CLANG-CL" CACHE STRING "" FORCE)
    message(STATUS "This is CLANG WITH MSVC Compatibility")
  elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    set (PLATFORM "${PLATFORM} MSVC" CACHE STRING "" FORCE)
  endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set (PLATFORM "${PLATFORM} CLANG" CACHE STRING "" FORCE)
  add_compile_options(-fcolor-diagnostics)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set (PLATFORM "${PLATFORM} GCC" CACHE STRING "" FORCE)
  add_compile_options(-fdiagnostics-color=always)
endif()

message(STATUS "CXX COMPILER: \t\t\t\t ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CXX PLATFORM: \t\t\t\t ${CMAKE_CXX_PLATFORM_ID}")
message(STATUS "RUNNING ON:   \t\t\t\t ${CMAKE_SYSTEM}")
list(POP_BACK CMAKE_MESSAGE_INDENT)