cmake_minimum_required(VERSION 3.14)
option(ENABLE_TIMSORT "Adds TimSort library" ON)

if(ENABLE_TIMSORT)
    message(STATUS  "TIMSORT:")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    message(VERBOSE "A C++ implementation of TimSort, an O(n log n) stable sorting algorithm, ported from Python's and OpenJDK's.. ")
    message(STATUS "+ FETCHING TIMSORT ....")

    include(FetchContent)
    FetchContent_Declare(
            timsort
            GIT_REPOSITORY https://github.com/timsort/cpp-TimSort.git
            GIT_TAG        v2.1.0)

    message(STATUS "+ CONFIGURING TIMSORT....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    FetchContent_MakeAvailable(timsort)

    list(POP_BACK CMAKE_MESSAGE_INDENT)
    message(STATUS "+ DONE")
    list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()