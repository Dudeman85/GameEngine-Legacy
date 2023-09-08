#include <engine/Application.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace engine;

//Creates a global instance of the ECS manager
ECS ecs;

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	//Paraeters define window size(x,y) and name
	GLFWwindow* window = CreateWindow(800, 600, "Window");
	//Initialize the default engine library
	EngineLib engine;
	//Create the camera
	Camera cam = Camera(800, 600);
	cam.perspective = true;
	cam.SetPosition(0, 0, 50);

	//changes window backround color
	SpriteRenderSystem::SetBackgroundColor(0, 120, 0);

	//Model loading
	Model model("assets/suzanne.obj");

	Entity suzanne = ecs.newEntity();
	Transform& suzanneTransform = ecs.addComponent(suzanne, Transform{  });
	ecs.addComponent(suzanne, ModelRenderer{ .model = &model });

	Entity suzanne2 = ecs.newEntity();
	Transform& suzanne2Transform = ecs.addComponent(suzanne2, Transform{  });
	ecs.addComponent(suzanne2, ModelRenderer{ .model = &model });

	//UI SYSTEM TESTING
	Texture tex("assets/strawberry.png");
	Entity strawberry = ecs.newEntity();
	ecs.addComponent(strawberry, Transform{ .position = Vector3(1, .1, 0), .scale = Vector3(.1, .1, 0) });
	ecs.addComponent(strawberry, SpriteRenderer{ .texture = &tex, .uiElement = true});

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//Close window when Esc is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			suzanneTransform.rotation.y += -1;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			suzanneTransform.rotation.y += 1;

		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		cam.Rotate(1, 0, 0);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}