#include <engine/Application.h>
#include <engine/TextRender.h>

using namespace std;
using namespace engine;

ECS ecs;



int main()
{
	// Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(800, 600, "Text rendering");
	
	// Camera
	Camera cam = Camera(800, 600);
	cam.SetPosition(0, 0, 10);

	// Initialize the default engine library
	//EngineLib engine;
	
	// Window background colour
	//SpriteRenderSystem::SetBackgroundColor(0, 120, 0);
	glClearColor(0 / 255, 120 / 255, 0 / 255, 1.0f);

	// Text
	TextRender text;
	text.LoadText();

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		// Update all engine systems, this usually should go last in the game loop
		// For greater control of system execution, you can update each one manually
		//engine.Update(&cam);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		text.RenderText(&cam, "This is sample text", 1, 1, 2.0f, glm::vec3(0.5, 0.8f, 0.2f));
		text.RenderText(&cam,"(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		// OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}