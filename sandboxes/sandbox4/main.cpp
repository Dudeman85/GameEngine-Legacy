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
	GLFWwindow* window = CreateWindow(1200, 800, "Window");
	//Initialize the default engine library
	EngineLib engine;
	//Create the camera
	Camera cam = Camera(1200, 800);
	cam.perspective = true;
	cam.SetPosition(0, 0, 50);

	//changes window backround color
	SpriteRenderSystem::SetBackgroundColor(29, 46, 119);

	//Model loading
	Model model("assets/achelous.obj");

	Entity ship = ecs.newEntity();
	Transform& modelTransform = ecs.addComponent(ship, Transform{ .rotation = Vector3(90, 0, 0), .scale = Vector3(1)});
	ecs.addComponent(ship, ModelRenderer{ .model = &model });
	//Angle of the ship, 0 is up
	float angle = 0;
	float rotationSpeed = 3;

	//UI SYSTEM TESTING
	Texture tex("assets/strawberry.png");
	Entity strawberry = ecs.newEntity();
	ecs.addComponent(strawberry, Transform{ .position = Vector3(1, .1, 0), .scale = Vector3(.1, .1, 0) });
	//ecs.addComponent(strawberry, SpriteRenderer{ .texture = &tex, .uiElement = true});

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//Close window when Esc is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			angle += rotationSpeed;
			if (angle >= 360)
				angle -= 360;

			cout << angle << endl;

			modelTransform.rotation.y += -rotationSpeed;
			modelTransform.rotation.z += rotationSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			angle -= rotationSpeed;
			if (angle < 0)
				angle += 360;

			cout << angle << endl;

			modelTransform.rotation.y += rotationSpeed;
			modelTransform.rotation.z += -rotationSpeed;
		}

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