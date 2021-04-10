#------------------------------------------------------------------------------
# Link-time optimization.
# Allows the linker to perform optimisations by reviewing tightly coupled
# code. For example if external symbol isn't used and references other
# symbols that are no longer used, then both can be removed. This enabled
# LTO for all build targets.
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.9)

# Enable Link-Time Optimization, AKA Inter-Procedure Optimization
if (NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
  include(CheckIPOSupported)
  check_ipo_supported(RESULT LTOAvailable OUTPUT output)
else()
  message(STATUS "LINK-TIME OPTIMIZATION: \t\t DISABLED FOR DEBUG BUILDS" )
  return()
endif ()

if (LTOAvailable)
  message(STATUS "LINK-TIME OPTIMIZATION: \t\t ON")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else()
  message(STATUS "LINK-TIME OPTIMIZATION: \t\t OFF" )
endif ()