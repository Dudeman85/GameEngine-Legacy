cmake_minimum_required(VERSION 3.20)
project(GameEngine)
option(ENGINE_BUILD_SANDBOXES "Build developer sandboxes" ON )
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("ext/OpenAL")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory("ext/libsndfile-master")
include_directories("include")
include_directories("ext/STB")
include_directories("ext/libsndfile-master/include")

# enet-1.3.17 library
add_subdirectory("ext/enet-1.3.17")
include_directories("ext/enet-1.3.17/include")

# freetype-2.13.1 library
add_subdirectory("ext/freetype-2.13.1")
include_directories("ext/freetype-2.13.1/include")

# tmxlite-master library
SET(TMXLITE_STATIC_LIB TRUE CACHE BOOL "")
add_subdirectory("ext/tmxlite-master/tmxlite")

# Assimp model loading library
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
add_subdirectory("ext/assimp-master")
#include_directories("ext/assimp-master/include")

# OpenGL
add_subdirectory("ext/glfw-3.3.2")
add_subdirectory("ext/glm-0.9.9.7")
include_directories("${GLFW_SOURCE_DIR}/deps")
set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" "${GLFW_SOURCE_DIR}/deps/glad_gl.c")

file(GLOB_RECURSE ENGINE_SOURCE_FILES "./src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADER_FILES "./include/engine/*.h" "./include/engine/GL/*.h" "./include/engine/AL/*.h")

# engine -library
add_library(engine ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES} ${GLAD_GL} ${STB})
target_link_libraries(engine glfw OpenAL tmxlite glm sndfile freetype assimp)
target_include_directories(engine PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# vertexShader copy
configure_file("bin/tilemapVertexShader.glsl" "sandboxes/sandbox1/tilemapVertexShader.glsl" COPYONLY)
configure_file("bin/tilemapVertexShader.glsl" "sandboxes/sandbox2/tilemapVertexShader.glsl" COPYONLY)
configure_file("bin/tilemapVertexShader.glsl" "sandboxes/sandbox3/tilemapVertexShader.glsl" COPYONLY)
configure_file("bin/tilemapVertexShader.glsl" "sandboxes/sandbox4/tilemapVertexShader.glsl" COPYONLY)
configure_file("bin/tilemapVertexShader.glsl" "sandboxes/sandbox5/tilemapVertexShader.glsl" COPYONLY)
configure_file("bin/tilemapVertexShader.glsl" "sandboxes/Demot/demo1/tilemapVertexShader.glsl" COPYONLY)
configure_file("bin/tilemapVertexShader.glsl" "sandboxes/Demot/demo2/tilemapVertexShader.glsl" COPYONLY)

# fragmentShader copy
configure_file("bin/tilemapFragmentShader.glsl" "sandboxes/sandbox1/tilemapFragmentShader.glsl" COPYONLY)
configure_file("bin/tilemapFragmentShader.glsl" "sandboxes/sandbox2/tilemapFragmentShader.glsl" COPYONLY)
configure_file("bin/tilemapFragmentShader.glsl" "sandboxes/sandbox3/tilemapFragmentShader.glsl" COPYONLY)
configure_file("bin/tilemapFragmentShader.glsl" "sandboxes/sandbox4/tilemapFragmentShader.glsl" COPYONLY)
configure_file("bin/tilemapFragmentShader.glsl" "sandboxes/sandbox5/tilemapFragmentShader.glsl" COPYONLY)
configure_file("bin/tilemapFragmentShader.glsl" "sandboxes/Demot/demo1/tilemapFragmentShader.glsl" COPYONLY)
configure_file("bin/tilemapFragmentShader.glsl" "sandboxes/Demot/demo2/tilemapFragmentShader.glsl" COPYONLY)

# OpenAL32 copy
IF (WIN32)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox1/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox2/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox3/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox4/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox5/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/Demot/demo1/OpenAL32.dll" COPYONLY)
configure_file("bin/OpenAL32.dll" "sandboxes/Demot/demo2/OpenAL32.dll" COPYONLY)
ENDIF()


# Sandboxes
if(ENGINE_BUILD_SANDBOXES)
	add_subdirectory("sandboxes")
endif()






