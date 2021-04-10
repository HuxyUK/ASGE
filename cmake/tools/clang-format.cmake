OPTION(ENABLE_CLANG_FORMAT "Adds clang-format style as a target" OFF)

if (ENABLE_CLANG_FORMAT)
  find_program(
      CFE NAMES clang-format clang-format-11 clang-format-10
      HINTS
      "${CMAKE_SOURCE_DIR}/tools/*/${PLATFORM}/"
      "$ENV{ProgramFiles}\\LLVM\\bin")

  if (CFE)
    execute_process(
        COMMAND ${CFE} --version
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE CFE_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    message(STATUS "CLANG-FORMAT VERSION: \t\t ${CFE_VERSION}")
    file(GLOB_RECURSE SOURCE_CODE
         LIST_DIRECTORIES FALSE
         CONFIGURE_DEPENDS
         ${PROJECT_SOURCE_DIR}/app*/*.[chi]pp
         ${PROJECT_SOURCE_DIR}/app*/*.[h]
         ${PROJECT_SOURCE_DIR}/app*/*.[chi]pp
         ${PROJECT_SOURCE_DIR}/app*/*.[h]
         ${PROJECT_SOURCE_DIR}/include/*.[chi]pp
         ${PROJECT_SOURCE_DIR}/include/*.[ch]
         ${PROJECT_SOURCE_DIR}/src/*.[chi]pp
         ${PROJECT_SOURCE_DIR}/src/*.[ch]
         ${PROJECT_SOURCE_DIR}/source/*.[chi]pp
         ${PROJECT_SOURCE_DIR}/source/*.[chi]pp)

    if (SOURCE_CODE)
      list(APPEND CLANG-FORMAT_ARGS
          -i
          --style=file
          ${SOURCE_CODE})

      add_custom_target(
          ClangFormat
          COMMAND "${CFE}" ${CLANG-FORMAT_ARGS}
          COMMENT "Code style analysis using ${CFE_VERSION}")
    endif ()
  endif ()
endif ()
