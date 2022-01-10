message(STATUS  "STDUUID C++17:")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
message(VERBOSE "STDUUID C++17 IMPLEMENTATION OF PROPOSAL P0959")

include(FetchContent)
message(STATUS "+ FETCHING STDUUID....")
FetchContent_Declare(
        stduuid
        GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
        GIT_TAG        d393e000378713bd0bfe07375adfa9d026e8645f)

FetchContent_GetProperties(stduuid)
if(NOT stduuid_POPULATED)
    set(UUID_BUILD_TESTS OFF  CACHE BOOL "" FORCE)
    set(UUID_USING_CXX20_SPAN ON CACHE BOOL "" FORCE)
    message(STATUS "+ CONFIGURING STDUUID....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    FetchContent_MakeAvailable(stduuid)
    add_library(stduuid::stduuid ALIAS stduuid)
    message(DEBUG "STDUUID INCLUDE DIR: ${stduuid_SOURCE_DIR}")

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(STATUS "+ DONE")
    list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()