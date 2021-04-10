#------------------------------------------------------------------------------
# Magic Enum C++
# Header-only C++17 library provides static reflection for enums.
# https://github.com/Neargye/magic_enum.git
#------------------------------------------------------------------------------
OPTION(ENABLE_MAGIC_ENUM "Adds Static Reflection for ENUMS" OFF)
if(ENABLE_MAGIC_ENUM)
  message(STATUS  "MAGIC-ENUM-C++:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  message(VERBOSE "Header-only C++17 library provides static reflection for enums")
  
  # todo: move this to cmake macro
  # fetch project
  include(FetchContent)
  message(STATUS "+ FETCHING MAGIC-ENUM-C++....")
  FetchContent_Declare(
      magic_enum
      GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
      GIT_TAG        origin/master)
  
  FetchContent_GetProperties(magic_enum)
  if(NOT magic_enum_POPULATED)
    message(STATUS "+ CONFIGURING MAGIC-ENUM-C++....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    FetchContent_Populate(magic_enum)
    
    # create a header only library
    add_library(magic_enum INTERFACE)
    add_library(magic_enum::magic_enum ALIAS magic_enum)
    target_include_directories(
        magic_enum
        SYSTEM INTERFACE
        ${magic_enum_SOURCE_DIR}/include)
    message(DEBUG "MAGIC_ENUM INCLUDE DIR: ${magic_enum_SOURCE_DIR}")
    
    # apparently this helps with some of dem IDEs
    target_sources(magic_enum INTERFACE ${magic_enum_SOURCE_DIR}/include/magic_enum.hpp)
    
    list(POP_BACK CMAKE_MESSAGE_INDENT)
  endif()
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()