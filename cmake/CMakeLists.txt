#------------------------------------------------------------------------------
# Add the build scripts
#------------------------------------------------------------------------------
file(GLOB_RECURSE build_scripts "build/*.cmake")
foreach(filename ${build_scripts})
    include(${filename})
endforeach()

#------------------------------------------------------------------------------
# Add the custom functions to help with builds
#------------------------------------------------------------------------------
file(GLOB_RECURSE macro_scripts "functions/*.cmake")
foreach(filename ${macro_scripts})
    include(${filename})
endforeach()

#------------------------------------------------------------------------------
# Add 3rd party library support
#------------------------------------------------------------------------------
message(STATUS "IMPORTING LIBRARIES")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
file(GLOB_RECURSE libs "libs/*.cmake")
foreach(filename ${libs})
    include(${filename})
endforeach()
list(POP_BACK CMAKE_MESSAGE_INDENT)
