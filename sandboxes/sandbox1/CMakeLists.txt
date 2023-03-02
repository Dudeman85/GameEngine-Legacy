add_executable(sandbox1 main.cpp SoundBuffer.h SoundBuffer.cpp SoundDevice.cpp SoundDevice.h SoundSource.cpp SoundSource.h MusicBuffer.cpp MusicBuffer.h)
target_include_directories(sandbox1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(sandbox1 engine)
