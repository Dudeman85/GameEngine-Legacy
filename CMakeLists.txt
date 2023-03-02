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
# tmxlite-master library
SET(TMXLITE_STATIC_LIB TRUE CACHE BOOL "")
add_subdirectory("ext/tmxlite-master/tmxlite")

# OpenGL
add_subdirectory("ext/glfw-3.3.2")
add_subdirectory("ext/glm-0.9.9.7")
include_directories("${GLFW_SOURCE_DIR}/deps")
set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" "${GLFW_SOURCE_DIR}/deps/glad_gl.c")

add_subdirectory("ext/box2d-main")
include_directories("ext/box2d-main/include")

file(GLOB_RECURSE ENGINE_SOURCE_FILES "./src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADER_FILES "./include/engine/*.h" "./include/engine/GL/*.h")

# engine -library
add_library(engine ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES} ${GLAD_GL} ${STB})
target_link_libraries(engine glfw OpenAL tmxlite glm sndfile box2d)
target_include_directories(engine PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
# OpenAL32 copy
IF (WIN32)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox1/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox2/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox3/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox4/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/sandbox5/OpenAL32.dll" COPYONLY)
	configure_file("bin/OpenAL32.dll" "sandboxes/Demot/demo1/OpenAL32.dll" COPYONLY)
ENDIF()


# Sandboxes
if(ENGINE_BUILD_SANDBOXES)
	add_subdirectory("sandboxes")
endif()






