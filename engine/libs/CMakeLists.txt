#OpenGL Preference
set(OpenGL_GL_PREFERENCE GLVND)

# GLFW Build Optionals
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

# GLFW Build
add_subdirectory(glfw)
if(GLFW_SHARED_LIB)
    target_compile_definitions(glfw PRIVATE BUILD_SHARED_LIBS=TRUE)
else()
    target_compile_definitions(glfw PRIVATE BUILD_SHARED_LIBS=FALSE)
endif()

add_dependencies(glfw update_mappings)
target_compile_options(glfw PRIVATE $<$<BOOL:${MSVC}>:/w>)
list(APPEND LINK_LIBS glfw ${GLFW_LIBRARIES})

# GLAD Build
add_subdirectory(glad)
list(APPEND LINK_LIBS glad)

# Freetype
add_subdirectory(freetype2)
add_library(Freetype::Freetype ALIAS freetype)
include(msdfgen/msdfgen.cmake)
if(MONOLITHIC_BUILD AND MSVC)
    set_property(TARGET msdfgen-core PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set_property(TARGET msdfgen-ext  PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()
list(APPEND LINK_LIBS "freetype" "msdfgen::msdfgen-core" "msdfgen::msdfgen-ext")


# PhysFS and PhysFS++
if(PHYSFS_SHARED_LIB)
    set (PHYSFS_BUILD_STATIC OFF)
    set (PHYSFS_BUILD_SHARED ON)
else()
    set (PHYSFS_BUILD_STATIC ON)
    set (PHYSFS_BUILD_SHARED OFF)
endif()

# PhysFS External Project
include(ExternalProject)

ExternalProject_Add(
        PhysFS
        PREFIX         "${CMAKE_CURRENT_BINARY_DIR}/physfs"
        SOURCE_DIR     "${CMAKE_CURRENT_LIST_DIR}/physfs"
        INSTALL_DIR    ""
        UPDATE_COMMAND ""
        CMAKE_ARGS     -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}}
                       -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                       -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
                       -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
                       -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
                       -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                       -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                       -DPHYSFS_BUILD_STATIC=ON
                       -DPHYSFS_BUILD_SHARED=OFF
                       -DPHYSFS_BUILD_TEST=OFF
)

# PhysFS++ Build
ExternalProject_Add(
        PhysFS++
        PREFIX         "${CMAKE_CURRENT_BINARY_DIR}/physfs++"
        SOURCE_DIR     "${CMAKE_CURRENT_LIST_DIR}/physfs++"
        INSTALL_DIR    ""
        UPDATE_COMMAND ""
        CMAKE_ARGS     -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}}
                       -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                       -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
                       -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
                       -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
                       -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                       -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
                       -DPHYSFS_BUILD_STATIC=${PHYSFS_BUILD_STATIC}
                       -DPHYSFS_BUILD_SHARED=${PHYSFS_BUILD_SHARED}
                       -DPHYSFS_HEADER_DIR=${CMAKE_CURRENT_BINARY_DIR}/physfs/include
                       -DPHYSFS_LIBRARY_DIR=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)

if(PHYSFS_SHARED_LIB OR NOT MSVC AND NOT WIN_CLANG)
    list(APPEND LINK_LIBS debug physfscppd optimized physfscpp)
    list(APPEND LINK_LIBS physfs)
else()
    list(APPEND LINK_LIBS debug physfscpp-staticd optimized physfscpp-static)
    list(APPEND LINK_LIBS physfs-static)
endif()

add_dependencies(PhysFS++ PhysFS)
set(LINK_LIBS ${LINK_LIBS} PARENT_SCOPE)