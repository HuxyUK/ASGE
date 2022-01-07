#------------------------------------------------------------------------------
# MSDFGEN
# Multi-channel signed distance field generator.
# https://github.com/Chlumsky/msdfgen
#------------------------------------------------------------------------------
OPTION(ENABLE_MSDFGEN "Adds SDF Font Rendering" OFF)
if(ENABLE_MSDFGEN)
  message(STATUS  "MSDFGEN:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  message(VERBOSE "Multi-channel signed distance field generator")
  
  # todo: move this to cmake macro
  # fetch project
  include(FetchContent)
  message(STATUS "+ FETCHING MSDFGEN....")
  FetchContent_Declare(
      msdfgen
      GIT_REPOSITORY https://github.com/Chlumsky/msdfgen.git
      GIT_TAG        origin/master)
  
  FetchContent_GetProperties(msdfgen)
  if(NOT msdfgen_POPULATED)
    set(MSDFGEN_BUILD_STANDALONE  OFF CACHE BOOL "" FORCE)
    set(MSDFGEN_INSTALL           OFF CACHE BOOL "" FORCE)
    set(MSDFGEN_USE_OPENMP        ON  CACHE BOOL "" FORCE)

    message(STATUS "+ CONFIGURING MSDFGEN....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    FetchContent_MakeAvailable(msdfgen)
    message(DEBUG "MSDFGEN INCLUDE DIR: ${msdfgen_SOURCE_DIR}")
    list(POP_BACK CMAKE_MESSAGE_INDENT)
  endif()

  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()