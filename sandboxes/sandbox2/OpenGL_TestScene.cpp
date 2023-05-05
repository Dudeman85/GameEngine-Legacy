#if 0


// Getting started ok!
// Audio Reference 
#include <engine/Application.h>

using namespace std;
using namespace engine;
ECS ecs;
int main() {
	// Create the window and OpenGL context before creating Enginelib
	// Values define window size(x,y) and name
	GLFWwindow* window = CreateWindow(800, 600, "Test Game");

	// Initialize the default engine library
	EngineLib engine;

	// Create the camera
	Camera cam = Camera(800, 600);

	// Changes window background color, values 0 - 1, RGB
	RenderSystem::SetBackgroundColor(0, .5, .1);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Close window when Esc is pressed
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
}
#endif