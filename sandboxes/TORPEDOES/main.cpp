#include <engine/Application.h>
#include "PlayerController.h"

using namespace engine;

ECS ecs;

int main()
{

	GLFWwindow* window = engine::CreateGLWindow(1300, 900, "Windos");

	engine::EngineLib engine;

	engine::Camera cam = engine::Camera(1300, 900);
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
	Entity laMuerte1 = ecs.newEntity();
	Entity laMuerte2 = ecs.newEntity();
	Entity laMuerte3 = ecs.newEntity();

	Transform& PLayerTransform = ecs.addComponent(laMuerte, Transform{ .position = Vector3(0,0,0), .rotation = Vector3(0,0,0), .scale = Vector3(60) });
	Transform& PLayerTransform1 = ecs.addComponent(laMuerte1, Transform{ .position = Vector3(200,-200,0), .rotation = Vector3(0,0,0), .scale = Vector3(60) });
	Transform& PLayerTransform2 = ecs.addComponent(laMuerte2, Transform{ .position = Vector3(300,300,0), .rotation = Vector3(0,0,0), .scale = Vector3(60) });
	Transform& PLayerTransform3 = ecs.addComponent(laMuerte3, Transform{ .position = Vector3(-100,-400,0), .rotation = Vector3(0,0,0), .scale = Vector3(60) });

	ecs.addComponent(laMuerte, ModelRenderer{ .model = &model });
	ecs.addComponent(laMuerte, Rigidbody{});
	ecs.addComponent(laMuerte, BoxCollider{});
	ecs.addComponent(laMuerte, Player{});

	ecs.addComponent(laMuerte1, ModelRenderer{ .model = &model });
	ecs.addComponent(laMuerte1, Rigidbody{});
	ecs.addComponent(laMuerte1, BoxCollider{});
	ecs.addComponent(laMuerte1, Player{});

	ecs.addComponent(laMuerte2, ModelRenderer{ .model = &model });
	ecs.addComponent(laMuerte2, Rigidbody{});
	ecs.addComponent(laMuerte2, BoxCollider{});
	ecs.addComponent(laMuerte2, Player{});

	ecs.addComponent(laMuerte3, ModelRenderer{ .model = &model });
	ecs.addComponent(laMuerte3, Rigidbody{});
	ecs.addComponent(laMuerte3, BoxCollider{});
	ecs.addComponent(laMuerte3, Player{});

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
