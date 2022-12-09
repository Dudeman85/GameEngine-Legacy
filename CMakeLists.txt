cmake_minimum_required(VERSION 3.20)
project(GameEngine)
option(ENGINE_BUILD_SANDBOXES "Build developer sandboxes" ON )
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("ext/OpenAL")
# SFML: https://www.sfml-dev.org/
# SFML Dokumentaatio: https://www.sfml-dev.org/documentation/2.5.1/
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory("ext/SFML-2.5.1")
include_directories("ext/SFML-2.5.1/include")
include_directories("include")
# tmxlite-master library
SET(TMXLITE_STATIC_LIB TRUE CACHE BOOL "")
add_subdirectory("ext/tmxlite-master/tmxlite")
add_subdirectory("ext/tmxlite-master/SFMLExample")

add_subdirectory("ext/box2d-main")
include_directories("ext/box2d-main/include")
# miniaudio
include_directories("ext")

file(GLOB_RECURSE ENGINE_SOURCE_FILES "./src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADER_FILES "./include/engine/*.h")

# engine -library
add_library(engine ${ENGINE_SOURCE_FILES} ${ENGINE_HEADER_FILES})
target_link_libraries(engine sfml-graphics OpenAL tmxlite)
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
ENDIF()

#set(ENGINE_LIBS engine sfml-graphics)

# Sandboxes
if(ENGINE_BUILD_SANDBOXES)
	add_subdirectory("sandboxes")
endif()






