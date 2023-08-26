// Text Rendering includes
#include <ft2build.h>
#include FT_FREETYPE_H

#include <engine/Application.h>

using namespace std;
using namespace engine;

ECS ecs;

int main()
{
	// Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(800, 600, "Text rendering");

	// Camera
	Camera cam = Camera(800, 600);

	// Initialize the default engine library
	EngineLib engine;

	// Window background colour
	RenderSystem::SetBackgroundColor(68, 154, 141);

	// Initialize FreeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
		return -1;
	}

	// Font load
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/ARIAL.TTF", 0, &face))
	{
		cout << "ERROR::FREETYPE: Failed to load font" << endl;
		return -1;
	}



	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		// Update all engine systems, this usually should go last in the game loop
		// For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		// OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}