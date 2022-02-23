cmake_minimum_required(VERSION 3.9.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
project(asge)
string(TIMESTAMP date "%Y%m%d")
set(ASGE_VERSION_MAJOR "3")
set(ASGE_VERSION_MINOR "2")
set(ASGE_VERSION_PATCH "0")
set(ASGE_VERSION_EXTRA "${date}")
set(ASGE_VERSION "${ASGE_VERSION_MAJOR}.${ASGE_VERSION_MINOR}")
set(ASGE_VERSION_FULL "${ASGE_VERSION}.${ASGE_VERSION_PATCH}${ASGE_VERSION_EXTRA}")

# Let's give the debug library a postfix
set(CMAKE_DEBUG_POSTFIX d)

# Setup the libs that the game engine are reliant on
list(APPEND LINK_LIBS "magic_enum::magic_enum" "stduuid::stduuid")
add_subdirectory(libs)

set( PROJECT_HEADERS
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Colours.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/FileIO.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Font.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Game.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/GameSettings.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Input.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/InputEvents.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Keys.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Logger.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Mouse.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/NonCopyable.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Camera.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/OGLGame.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/PixelBuffer.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Point2D.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/RenderTarget.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Renderer.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Resolution.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Shader.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Sprite.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Texture.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Text.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Viewport.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/include/Engine/Value.hpp" )

set( PROJECT_SOURCES
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/FileIO.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Font.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Game.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Input.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Logger.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/SplashScreen.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/SplashScreen.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Camera.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OGLGame.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Point2D.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Renderer.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Resolution.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Shader.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Sprite.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/Text.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/CGLSpriteRenderer.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/CGLSpriteRenderer.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLAtlas.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLAtlas.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLConstants.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLFontSet.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLFontSet.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLFormat.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLIncludes.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLInput.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLInput.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLQuad.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLQuad.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLPixelBuffer.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLPixelBuffer.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLRenderBatch.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLRenderBatch.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLRenderState.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLRenderTarget.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLRenderTarget.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLRenderer.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLRenderer.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLShader.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLShader.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLSprite.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLSprite.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLSpriteBatch.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLSpriteBatch.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLModernSpriteRenderer.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLModernSpriteRenderer.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLAtlasManager.h"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLAtlasManager.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLTexture.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLTexture.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLTextureCache.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLTextureCache.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLLegacySpriteRenderer.hpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLLegacySpriteRenderer.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/src/Engine/OpenGL/GLWindowData.hpp")

set(PROJECT_CONFIG
    CMakeLists.txt LICENSE
	README.md Doxyfile Doxyfile-mcss
	.gitattributes .gitignore .gitmodules .clang_format)

message("${PLATFORM_SOURCES}")

source_group("Configs"  FILES ${PROJECT_CONFIG})
source_group("Headers"  FILES ${PROJECT_HEADERS})
source_group("Platform" FILES ${PLATFORM_SOURCES})
source_group("Sources"  FILES ${PROJECT_SOURCES})
source_group("Vendors"  FILES ${VENDORS_SOURCES})

add_definitions(
        -DGLFW_INCLUDE_NONE
        -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

if(ENGINE_SHARED_LIB)
    add_library(
		${PROJECT_NAME} SHARED src/Engine/Game.cpp)
else()
	add_library(
		${PROJECT_NAME} STATIC src/Engine/Game.cpp)
endif()

target_sources( ${PROJECT_NAME}
		PUBLIC
        ${PROJECT_HEADERS}

        PRIVATE
		${PROJECT_SOURCES}
		${PLATFORM_SOURCES})


# Define headers for this library. PUBLIC headers are used for
# compiling the library and added to build paths. 
target_include_directories( ${PROJECT_NAME}
		SYSTEM
		INTERFACE
        $<INSTALL_INTERFACE:include>
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
		PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/physfs++/include>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/physfs/src>"
      	PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/include/Engine"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Engine"
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/glm"
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/freetype2/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/libs/stb"
        )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
		asgeConfigVersion.cmake
		VERSION ${ASGE_VERSION_FULL}
		COMPATIBILITY AnyNewerVersion
)

# Compiler requirements for this library,
target_compile_features( ${PROJECT_NAME} PUBLIC cxx_std_20 )

# Compilation settings
target_compile_options(
		${PROJECT_NAME} PRIVATE
		"$<$<CXX_COMPILER_ID:Clang>:-Wpadded>"
		"$<$<STREQUAL:${CMAKE_CXX_SIMULATE_ID},MSVC>:/EHsc>"
		"$<$<CXX_COMPILER_ID:MSVC>:/EHsc>")

# Preprocessor requirements
if(PHYSFS_SHARED_LIB)
    target_compile_definitions( ${PROJECT_NAME} PRIVATE PHYSFS_EXPORT_LIB GLM_FORCE_CXX17 GLM_FORCE_CTOR_INIT)
else()
    target_compile_definitions( ${PROJECT_NAME} PRIVATE PHYSFS_CPP_STATIC GLM_FORCE_CXX17 GLM_FORCE_CTOR_INIT )
endif()

# Dependencies on other libraries
target_link_libraries( ${PROJECT_NAME} PUBLIC ${LINK_LIBS})
add_dependencies( ${PROJECT_NAME} PhysFS++)

target_compile_definitions(
		${PROJECT_NAME}
		PRIVATE
		"$<$<CONFIG:Debug>:NO_SPLASH>"
		"$<$<CONFIG:Debug>:GL_CHECK_ERRORS>"
		"$<$<CONFIG:Debug>:LOGGING_LEVEL_ALL>"
		"$<$<CONFIG:Release>:LOGGING_LEVEL_ERROR>"
		"$<$<CXX_COMPILER_ID:MSVC>:__PRETTY_FUNCTION__=__FUNCSIG__>"
		"$<$<STREQUAL:${CMAKE_CXX_SIMULATE_ID},MSVC>:_ENABLE_EXTENDED_ALIGNED_STORAGE>")

set_target_properties(
        ${PROJECT_NAME} PROPERTIES
		VERSION ${ASGE_VERSION_FULL}
		SOVERSION ${ASGE_VERSION}
        LINKER_LANGUAGE CXX
        CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/build
        CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
        )

target_link_directories(
		${PROJECT_NAME} PRIVATE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)

#add_custom_command(
#		TARGET ${PROJECT_NAME} POST_BUILD
#		COMMAND ${CMAKE_COMMAND} -E copy
#		${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/OpenGL/Shaders/Vertex.glsl
#		${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/OpenGL/Shaders/Frag.glsl
#		${CMAKE_BINARY_DIR}/bin)
