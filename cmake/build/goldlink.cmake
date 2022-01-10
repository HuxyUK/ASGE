#------------------------------------------------------------------------------
# Gold is a linker for ELF files. This script will enable its use.
# The motivation for writing gold was to make a linker that is faster than the
# GNU linker, especially for large applications coded in C++.
#------------------------------------------------------------------------------
if (NOT APPLE)
  execute_process(
      COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -W --version
      ERROR_QUIET
      OUTPUT_VARIABLE ld_version)
  
  #  if ("${ld_version}" MATCHES "GNU gold")
  option(USE_GOLD_LINKER "Whether the gold linker should be used" OFF)
  message(STATUS "USE GOLD LINKING: \t \t \t ${USE_GOLD_LINKER}")
  
  if(USE_GOLD_LINKER)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold -Wl,--disable-new-dtags")
  endif()
  # endif()
endif()
