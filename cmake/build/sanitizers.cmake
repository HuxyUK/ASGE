#------------------------------------------------------------------------------
# Clang and GCC sanitzers.
#------------------------------------------------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message(STATUS "SANITIZERS:")
  
  option(ADDRESS_SANITIZER "description" OFF)
  message(STATUS "  + ADDRESS_SANITIZER \t\t ${ADDRESS_SANITIZER}")
  if(ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    link_libraries(-fsanitize=address -fno-omit-frame-pointer)
  endif()
  
  option(UB_SANITIZER "description" OFF)
  message(STATUS "  + UB_SANITIZER \t\t\t ${UB_SANITIZER}")
  if(UB_SANITIZER)
    add_compile_options(-fsanitize=undefined)
    link_libraries(-fsanitize=undefined)
  endif()
  
  option(THREAD_SANITIZER "description" OFF)
  message(STATUS "  + THREAD_SANITIZER \t\t ${THREAD_SANITIZER}")
  if(THREAD_SANITIZER)
    add_compile_options(-fsanitize=undefined)
    link_libraries(-fsanitize=undefined)
  endif()
  
  # Clang only
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    option(MEMORY_SANITIZER "description" OFF)
    message(STATUS "  + MEMORY_SANITIZER \t\t ${MEMORY_SANITIZER}")
    if(MEMORY_SANITIZER AND NOT ADDRESS_SANITIZER)
      add_compile_options(-fsanitize=memory -fno-omit-frame-pointer)
      link_libraries(-fsanitize=memory -fno-omit-frame-pointer)
    endif()
  endif()
endif()

