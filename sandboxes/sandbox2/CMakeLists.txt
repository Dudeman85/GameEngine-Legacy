add_executable(sandbox2 Main_OpenGL_TILED.cpp Aaro_TestScene.cpp Game.cpp Game.hpp GLCheck.hpp main.cpp MapLayer.cpp MapLayer.hpp Shader.inl ${GLAD_GL})
target_link_libraries(sandbox2 engine)