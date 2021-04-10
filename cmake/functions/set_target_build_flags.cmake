function(set_target_build_flags tgt)
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(
            ${tgt} PRIVATE
            "-Wall" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor"
            "-Wold-style-cast" "-Wunused" "-pedantic"
            "-Woverloaded-virtual" "-Wpedantic" "-Wconversion"
            "-Wduplicated-cond" "-Wduplicated-branches" "-Wlogical-op"
            "-Wnull-dereference" "-Wuseless-cast" "-Wredundant-move"
            "-Wpessimizing-move" "-Werror"
            $<$<BOOL:$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>>:-O0>)

  elseif (${PLATFORM} MATCHES "CLANG")
    target_compile_options(
            ${tgt} PRIVATE
            "-Wall" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor"
            "-Wold-style-cast" "-Wunused" "-Wno-documentation"
            "-Woverloaded-virtual" "-Wpedantic" "-Wconversion"
            "-Wdouble-promotion" "-Wundef" "-Wnull-dereference"
            "-Wno-c++98-compat" "-Wredundant-move"
            "-Wpessimizing-move" "-Wno-c++98-compat-pedantic"
            "-Wno-documentation-unknown-command" "-Wno-double-promotion"
            "-Werror" "-ferror-limit=100" )

  elseif (${PLATFORM} MATCHES "MSVC")
    target_compile_options(${tgt} PRIVATE /Wall /WX)
  endif()
endfunction()
