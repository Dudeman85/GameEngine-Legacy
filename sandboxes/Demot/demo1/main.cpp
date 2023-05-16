
#include <engine/Tilemap.h>

#include "PlayerController.h"

#include <chrono>
#include <thread>

using namespace std;
using namespace engine;

ECS ecs;

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(800, 600, "Window");

	//Initialize the default engine library
	EngineLib engine;

	//Register Player Controller
	ecs.registerComponent<Player>();
	shared_ptr<PlayerController> playerController = ecs.registerSystem<PlayerController>();
	Signature playerControllerSignature;
	playerControllerSignature.set(ecs.getComponentId<Transform>());
	playerControllerSignature.set(ecs.getComponentId<Player>());
	playerControllerSignature.set(ecs.getComponentId<Sprite>());
	playerControllerSignature.set(ecs.getComponentId<Rigidbody>());
	playerControllerSignature.set(ecs.getComponentId<BoxCollider>());
	ecs.setSystemSignature<PlayerController>(playerControllerSignature);

	engine.physicsSystem->gravity = Vector2(0, -10000);
	engine.physicsSystem->step = 4;

	//Create the camera
	Camera cam = Camera(800, 600);

	//Load a new texture
	Texture texture = Texture("assets/strawberry.png");

	//Create a new entity
	Entity player = ecs.newEntity();
	Transform& playerTransform = ecs.addComponent(player, Transform{ .x = 10, .y = 25, .z = 0, .xScale = 60, .yScale = 60 });
	ecs.addComponent(player, Sprite{});
	ecs.addComponent(player, Animator{});
	ecs.addComponent(player, Rigidbody{ .gravityScale = 1, .drag = 0, .friction = 0.0, .elasticity = 0 });
	ecs.addComponent(player, BoxCollider{ .scale = Vector2(0.5, 0.65), .offset = Vector2(0, -18.5) });
	ecs.addComponent(player, Player{});
	BoxCollider& playerCollider = ecs.getComponent<BoxCollider>(player);
	Rigidbody& playerRigidbody = ecs.getComponent<Rigidbody>(player);

	//Define the test animation
	Animator& animator = ecs.getComponent<Animator>(player);
	auto testAnims = AnimationsFromSpritesheet("assets/warriorsheet.png", 8, 5, vector<int>(8 * 5, 100));
	AnimationSystem::AddAnimations(player, testAnims, vector<string>{"Idle", "Run", "Wallslide", "Jump", "Attack 1"});
	AnimationSystem::PlayAnimation(player, "Idle", true);

	//Top-Right
	Entity sprite2 = ecs.newEntity();
	ecs.addComponent(sprite2, Transform{ .x = 300, .y = 200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite2, Sprite{ &texture });
	ecs.addComponent(sprite2, Rigidbody{ .kinematic = true });
	ecs.addComponent(sprite2, BoxCollider{ .scale = Vector2(10, 1) });
	//Bottom-Left
	Entity sprite3 = ecs.newEntity();
	ecs.addComponent(sprite3, Transform{ .x = -300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite3, Sprite{ &texture });
	ecs.addComponent(sprite3, Rigidbody{ .kinematic = true });
	ecs.addComponent(sprite3, BoxCollider{});
	Entity sprite6 = ecs.newEntity();
	ecs.addComponent(sprite6, Transform{ .x = -320, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite6, Sprite{ &texture });
	ecs.addComponent(sprite6, Rigidbody{ .kinematic = true });
	ecs.addComponent(sprite6, BoxCollider{});
	//Top-Left
	Entity sprite4 = ecs.newEntity();
	ecs.addComponent(sprite4, Transform{ .x = -310, .y = 200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite4, Sprite{ &texture });
	ecs.addComponent(sprite4, Rigidbody{ .gravityScale = 0, .drag = 0.1, .friction = 0.2, .elasticity = 0.125, .kinematic = false });
	ecs.addComponent(sprite4, BoxCollider{});
	//Bottom-Right
	Entity sprite5 = ecs.newEntity();
	ecs.addComponent(sprite5, Transform{ .x = 300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite5, Sprite{ &texture });
	ecs.addComponent(sprite5, Rigidbody{ .velocity = Vector2(-985, 2000), .drag = 0.25, .elasticity = 0.625, .kinematic = false });
	ecs.addComponent(sprite5, BoxCollider{});

	RenderSystem::SetBackgroundColor(0, .5, .1);

	Tilemap map(&cam);
	map.loadMap("assets/level.tmx");

	engine.physicsSystem->SetTilemap(&map);

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds{100});

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (playerCollider.sidesCollided[Direction::up])
		{
			cout << "a";
		}


		playerController->Update(window, engine.deltaTime);

		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		map.draw();
		cam.SetPosition(playerTransform.x, playerTransform.y, + 100);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}