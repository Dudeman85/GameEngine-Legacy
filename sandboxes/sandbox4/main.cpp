#include <engine/Application.h>
#include <engine/GL/Model.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Creates a global instance of the ECS manager
ECS ecs;

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	//Paraeters define window size(x,y) and name
	GLFWwindow* window = engine::CreateWindow(800, 600, "Window");
	//Initialize the default engine library
	engine::EngineLib engine;
	//Create the camera
	engine::Camera cam = engine::Camera(800, 600);

	//changes window backround color, values 0-1, RGB
	engine::RenderSystem::SetBackgroundColor(0, .5, .1);


	//MODEL TESTING
	//engine::Model suzanne("assets/suzanne.obj");
	engine::Shader s;


	//This gives 566 linker errors
	//Assimp::Importer importer;


	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//Close window when Esc is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//suzanne.Draw(s);

		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}