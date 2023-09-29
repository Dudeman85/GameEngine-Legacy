#include <engine/Application.h>
#include "PlayerController.h"

using namespace engine;

ECS ecs;

int main()
{

	GLFWwindow* window = engine::CreateGLWindow(800, 600, "Windos");

	engine::EngineLib engine;

	engine::Camera cam = engine::Camera(800, 600);
	cam.SetPosition(0, 0, 1000);
	cam.SetRotation(45, 0, 0);
   //engine.physicsSystem->gravity = Vector2(0, -981);
	ecs.registerComponent<Player>();
	shared_ptr<PlayerController>playerController = ecs.registerSystem< PlayerController>();
	Signature playerControllerSignature;
	playerControllerSignature.set(ecs.getComponentId< Transform>());
	playerControllerSignature.set(ecs.getComponentId< Player>());
	playerControllerSignature.set(ecs.getComponentId< Rigidbody>());
	playerControllerSignature.set(ecs.getComponentId< BoxCollider>());
	playerControllerSignature.set(ecs.getComponentId< ModelRenderer>());
	ecs.setSystemSignature<PlayerController>(playerControllerSignature);
	Model model("assets/LaMuerte.obj");

	Entity laMuerte = ecs.newEntity();

	Transform& PLayerTransform = ecs.addComponent(laMuerte, Transform{ .position = Vector3(0,0,0), .rotation = Vector3(0,0,0), .scale = Vector3(60) });
	ecs.addComponent(laMuerte, ModelRenderer{ .model = &model });
	ecs.addComponent(laMuerte, Rigidbody{});
	ecs.addComponent(laMuerte, BoxCollider{});
	ecs.addComponent(laMuerte, Player{});

	engine.modelRenderSystem->lightPos = Vector3(0, 200, 200);
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);



		engine.Update(&cam);
		playerController->Update(window);
	
		glfwSwapBuffers(window);
		glfwPollEvents();

	}



	glfwTerminate();
	return 0;
}
