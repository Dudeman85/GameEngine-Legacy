add_executable(sandbox2 Main_OpenGL_TILED.cpp Aaro_TestScene.cpp GLCheck.hpp main.cpp TiledMapLayer.hpp Shader.inl ${GLAD_GL})
target_link_libraries(sandbox2 engine)