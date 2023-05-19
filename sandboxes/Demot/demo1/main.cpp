#include <engine/Application.h>
#include <engine/Tilemap.h>

#include "PlayerController.h"
#include "PickupController.h"
#include "EnemyController.h"
#include "TurretController.h"

#include <chrono>
#include <thread>

using namespace std;
using namespace engine;

ECS ecs;

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(1000, 800, "Window");

	//Create the camera
	Camera cam = Camera(1000, 800);

	//Initialize the default engine library
	EngineLib engine;

	//Register Custom Systems
	//Register Player Controller
	ecs.registerComponent<Player>();
	shared_ptr<PlayerController> playerController = ecs.registerSystem<PlayerController>();
	Signature playerControllerSignature;
	playerControllerSignature.set(ecs.getComponentId<Transform>());
	playerControllerSignature.set(ecs.getComponentId<Player>());
	playerControllerSignature.set(ecs.getComponentId<Sprite>());
	playerControllerSignature.set(ecs.getComponentId<Rigidbody>());
	playerControllerSignature.set(ecs.getComponentId<BoxCollider>());
	playerControllerSignature.set(ecs.getComponentId<Animator>());
	ecs.setSystemSignature<PlayerController>(playerControllerSignature);
	//Register Pickup Controller
	ecs.registerComponent<Pickup>();
	shared_ptr<PickupController> pickupController = ecs.registerSystem<PickupController>();
	Signature pickupControllerSignature;
	pickupControllerSignature.set(ecs.getComponentId<Transform>());
	pickupControllerSignature.set(ecs.getComponentId<Pickup>());
	pickupControllerSignature.set(ecs.getComponentId<Sprite>());
	pickupControllerSignature.set(ecs.getComponentId<Rigidbody>());
	pickupControllerSignature.set(ecs.getComponentId<BoxCollider>());
	ecs.setSystemSignature<PickupController>(pickupControllerSignature);
	//Register Enemy Controller
	ecs.registerComponent<Enemy>();
	shared_ptr<EnemyController> enemyController = ecs.registerSystem<EnemyController>();
	Signature enemyControllerSignature;
	enemyControllerSignature.set(ecs.getComponentId<Transform>());
	enemyControllerSignature.set(ecs.getComponentId<Enemy>());
	enemyControllerSignature.set(ecs.getComponentId<Sprite>());
	enemyControllerSignature.set(ecs.getComponentId<Rigidbody>());
	enemyControllerSignature.set(ecs.getComponentId<BoxCollider>());
	enemyControllerSignature.set(ecs.getComponentId<Animator>());
	ecs.setSystemSignature<EnemyController>(enemyControllerSignature);
	//Register Turret Controller
	ecs.registerComponent<Turret>();
	shared_ptr<TurretController> turretController = ecs.registerSystem<TurretController>();
	Signature turretControllerSignature;
	turretControllerSignature.set(ecs.getComponentId<Transform>());
	turretControllerSignature.set(ecs.getComponentId<Turret>());
	turretControllerSignature.set(ecs.getComponentId<Sprite>());
	turretControllerSignature.set(ecs.getComponentId<Rigidbody>());
	turretControllerSignature.set(ecs.getComponentId<BoxCollider>());
	turretControllerSignature.set(ecs.getComponentId<Animator>());
	ecs.setSystemSignature<TurretController>(turretControllerSignature);

	static SoundSource speaker;

	//Create the player entity
	Entity player = ecs.newEntity();
	Transform& playerTransform = ecs.addComponent(player, Transform{ .x = 110, .y = 200, .z = 1, .xScale = 50, .yScale = 50 });
	ecs.addComponent(player, Sprite{});
	ecs.addComponent(player, Animator{});
	ecs.addComponent(player, Rigidbody{});
	ecs.addComponent(player, BoxCollider{ .scale = Vector2(0.2, 0.625), .offset = Vector2(0, -18) });
	ecs.addComponent(player, Player{});

	//Add animation to player
	vector<Animation> anims = AnimationsFromSpritesheet("assets/warriorsheet.png", 8, 4, vector<int>(8 * 4, 100));
	AnimationSystem::AddAnimations(player, anims, vector<string>{"Idle", "Run", "Wallslide", "Jump"});
	vector<Animation> attackAnims = AnimationsFromSpritesheet("assets/warriorattack.png", 4, 4, vector<int>(4 * 4, 100));
	AnimationSystem::AddAnimations(player, attackAnims, vector<string>{"Jump Attack", "Attack 1", "Attack 2", "Attack 3"});

	Entity waterfall = ecs.newEntity(); 
	ecs.addComponent(waterfall, Transform{ .x = 300, .y = -200, .z = 10, .xScale = 80, .yScale = 40 });
	ecs.addComponent(waterfall, Sprite{});
	ecs.addComponent(waterfall, Animator{});
	vector<Animation> waterfallAnims = AnimationsFromSpritesheet("assets/waterfall01.png", 4, 1, vector<int>(4 * 1, 100));
	AnimationSystem::AddAnimation(waterfall, waterfallAnims[0], "1");
	AnimationSystem::PlayAnimation(waterfall, "1", true);

	RenderSystem::SetBackgroundColor(0, .5, .1);

	Tilemap map(&cam);
	map.loadMap("assets/level01.tmx");

	//Set the gravity and tilemap collider
	engine.physicsSystem->gravity = Vector2(0, -10000);
	engine.physicsSystem->SetTilemap(&map);

	speaker.setLinearDistance(1, 1.f, 100.f, 600.f, 1.f);

	pickupController->CreatePickup(1780, -840);
	pickupController->CreatePickup(915, -420);
	pickupController->CreatePickup(624, -1250);
	pickupController->CreatePickup(1750, -1505);

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds{100});

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
			cout << playerTransform.x << ", " << playerTransform.y << endl;

		playerController->Update(window, engine.deltaTime, speaker);
		pickupController->Update(player, engine.programTime);

		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		map.draw();

		//Keep the camera in bounds of the tilemap
		float camPosX = clamp(playerTransform.x, map.position.x + cam.width / 2, map.position.x + map.bounds.width - cam.width / 2);
		float camPosY = clamp(playerTransform.y, map.position.y - map.bounds.height + cam.width / 2, map.position.y - cam.width / 2);
		cam.SetPosition(camPosX, camPosY, 100);

		engine.soundDevice->SetLocation(playerTransform.x, playerTransform.y, 0);
		engine.soundDevice->SetSourceLocation(1, playerTransform.x, playerTransform.y, 0);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}