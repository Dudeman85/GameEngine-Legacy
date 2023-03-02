#include <engine/Application.h>

using namespace std;
using namespace engine;

ECS ecs;

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(800, 600, "Window");

	//Initialize the default engine library
	EngineLib engine;

	//Create the camera
	Camera cam = Camera(800, 600);

	//Load a new texture
	Texture texture = Texture("strawberry.png");

	//Create a new entity
	Entity sprite = ecs.newEntity();
	ecs.addComponent(sprite, Transform{ .x = 0, .y = 0, .xScale = 50, .yScale = 50 });
	ecs.addComponent(sprite, Sprite{ .texture = &texture });

	//Create a new entity
	Entity sprite2 = ecs.newEntity();
	ecs.addComponent(sprite2, Transform{ .x = 200, .y = 100, .xScale = 50, .yScale = 50 });
	ecs.addComponent(sprite2, Sprite{ .texture = &texture });

	engine.renderSystem->SetBackgroundColor(0, .5, .1);

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			cam.Translate(1, 0);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			cam.Translate(-1, 0);
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			cam.Translate(0, 1);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			cam.Translate(0, -1);

		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
			cam.Translate(0, 0, -1);
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
			cam.Translate(0, 0, 1);


		//Update all engine systems
		engine.Update(&cam);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}