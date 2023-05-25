//#if 0
#include <engine/Application.h>
#include <engine/Tilemap.h>
#include <vector>
#include <set>
#include <chrono>
#include <thread>
#include "PlayerController.h"
#include "PickupController.h"
#include "TurretController.h"

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

	ecs.registerComponent<Player>();
	shared_ptr<PlayerController> playerController = ecs.registerSystem<PlayerController>();
	Signature playerControllerSignature;
	playerControllerSignature.set(ecs.getComponentId<Transform>());
	playerControllerSignature.set(ecs.getComponentId<Player>());
	playerControllerSignature.set(ecs.getComponentId<Sprite>());
	playerControllerSignature.set(ecs.getComponentId<Rigidbody>());
	playerControllerSignature.set(ecs.getComponentId<BoxCollider>());
	ecs.setSystemSignature<PlayerController>(playerControllerSignature);
	//Register Turret Controller
	ecs.registerComponent<Turret>();
	shared_ptr<TurretController> turretController = ecs.registerSystem<TurretController>();
	Signature turretControllerSignature;
	turretControllerSignature.set(ecs.getComponentId<Transform>());
	turretControllerSignature.set(ecs.getComponentId<Turret>());
	turretControllerSignature.set(ecs.getComponentId<Sprite>());
	turretControllerSignature.set(ecs.getComponentId<Rigidbody>());
	turretControllerSignature.set(ecs.getComponentId<BoxCollider>());
	ecs.setSystemSignature<TurretController>(turretControllerSignature);
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

	engine.physicsSystem->gravity = Vector2(0, 0);
	engine.physicsSystem->step = 8;


	static SoundSource mySpeaker1;
	static SoundSource mySpeaker2;
	static SoundSource mySpeaker3;
	static SoundSource mySpeaker4;

	///////////////////Texture & audio loading////////////////////

	Texture texture = Texture("assets/Hull.png");
	Texture turretTexture = Texture("assets/Gun_01.png");
	Texture texture2 = Texture("assets/crosshairEdit.png");
	Texture texture3 = Texture("assets/bullet.png");

	//load sound from file
	uint32_t sound1 = SoundBuffer::getFile()->addSoundEffect("assets/enginemono.wav");
	uint32_t sound2 = SoundBuffer::getFile()->addSoundEffect("assets/bang_05.wav");

	mySpeaker1.setLinearDistance(1, 0.1f, 10.f, 60.f, 1.f);
	mySpeaker2.setLinearDistance(2, 1.5f, 100.f, 700.f, 0.5f);
	mySpeaker3.setLinearDistance(3, 1.f, 10.f, 600.f, 1.f);



	//Create a new entity
	Entity player = ecs.newEntity();
	Transform& playerTransform = ecs.addComponent(player, Transform{ .x = 168, .y = -150, .z = 1.5, .xScale = 40, .yScale = 40 });
	ecs.addComponent(player, Sprite{ &texture });
	ecs.addComponent(player, Player{});
	Rigidbody& playerRigidbody = ecs.addComponent(player, Rigidbody{ .gravityScale = 1, .drag = 0, .friction = 0.0, .elasticity = 0 });
	BoxCollider& playerCollider = ecs.addComponent(player, BoxCollider{});

	Entity playerTurret = ecs.newEntity();
	Transform& playerTurretTransform = ecs.addComponent(playerTurret, Transform{ .xScale = 50, .yScale = 16.5 });
	ecs.addComponent(playerTurret, Sprite{ .texture = &turretTexture });

	//play sound files
	mySpeaker1.Play(sound1);
	mySpeaker1.SetLooping(1);

	turretController->player = player;
	turretController->CreateTurret(1000, -1000);
	turretController->CreateTurret(360, -690);
	turretController->CreateTurret(1395, -1010);
	turretController->CreateTurret(1448, -376);
	turretController->CreateTurret(1066, -168);
	turretController->CreateTurret(350, -1280);

	// create entity crosshair for gamepad
	Entity crosshair = ecs.newEntity();
	// adds crosshair texture
	Transform crosshairTransform = ecs.addComponent(crosshair, Transform{ .x = 500, .y = 500, .xScale = 20, .yScale = 20 });
	ecs.addComponent(crosshair, Sprite{ &texture2 });

	set<Entity>bullets;


	float fireCooldown = 0.04f;
	bool canFire = true;

	RenderSystem::SetBackgroundColor(68, 154, 141);
	Tilemap map(&cam);
	map.loadMap("assets/demo2.tmx");
	engine.physicsSystem->SetTilemap(&map);
	engine.renderSystem->SetTilemap(&map);

	pickupController->CreatePickup(600, -75);
	pickupController->CreatePickup(325, -1400);
	pickupController->CreatePickup(1340, -1375);
	pickupController->CreatePickup(1450, -100);

	Animation explosion = AnimationsFromSpritesheet("assets/explosion.png", 5, 1, vector<int>(5, 75))[0];

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{


		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		{
			cout << playerTransform.x << ", " << playerTransform.y << endl;
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		playerController->Update(window, engine.deltaTime, engine.physicsSystem);
		pickupController->Update(player);

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////OHJAINSÄÄDÖT////////////////////////////////////////////////////
		int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
		if (present == GLFW_TRUE)
		{

			GLFWgamepadstate state;

			// set player speed
			float playerSpeed = 500.0f;
			//define deadzone
			float deadzoneSize = 0.5f;
			//calculate deadzone barrier
			float deadzoneTreshold = deadzoneSize / 2.0f;
			//Camera max distance
			float max_distance = 30.0f;


			int axisCount;
			int buttonCount;
			float radian = 5.0f;

			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
			const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);


			if (abs(axes[0]) > deadzoneTreshold || abs(axes[1]) > deadzoneTreshold)
			{
				// Sets movement vector based on movement speed
				Vector2 movement(axes[0] * playerSpeed, -axes[1] * playerSpeed);

				// move player based on movement vector
				engine.physicsSystem->Move(player, movement * engine.deltaTime);
				//engine.physicsSystem->Move(hull, movement * engine.deltaTime);

				Vector2 leftThumbstick(axes[0], axes[1]);
				leftThumbstick = (leftThumbstick + Vector2(1.0f, 1.0f)) / 2.0f;
				leftThumbstick -= Vector2(0.5f, 0.5f);
				leftThumbstick *= 2.0f;

				playerTransform.zRotation = atan2f(-leftThumbstick.y, leftThumbstick.x) * 180 / 3.14f;
			}

			if (abs(axes[2]) > deadzoneTreshold || abs(axes[3]) > deadzoneTreshold)
			{

				// Changes joystick cordinates into origo and scales it between -1, 1
				Vector2 rightThumbstick(axes[2], axes[3]);
				rightThumbstick = (rightThumbstick + Vector2(1.0f, 1.0f)) / 2.0f;
				rightThumbstick -= Vector2(0.5f, 0.5f);
				rightThumbstick *= 2.0f;

				// creates vector toward joystick direction
				float angle = atan2f(rightThumbstick.y, rightThumbstick.x);
				Vector2 aimdirection = Vector2(cosf(angle), sinf(angle));

				// normalizes vector and sets crosshair distance
				aimdirection.Normalize();
				aimdirection *= 150.0f;
				ecs.getComponent<Sprite>(crosshair).enabled = true;
				// crosshair position based on player position
				Vector3 playerPosition(playerTransform.x, playerTransform.y, playerTransform.z);
				Vector3 crosshairPosition = playerPosition + Vector3(aimdirection.x, -aimdirection.y, 10);
				TransformSystem::SetPosition(crosshair, crosshairPosition);

				playerTurretTransform.zRotation = atan2f(-rightThumbstick.y, rightThumbstick.x) * 180 / 3.14f;

				if (fireCooldown <= 0)
				{
					if (axes[5] > 0.5f)
					{

						Entity bullet = ecs.newEntity();
						ecs.addComponent(bullet, Transform{ .x = playerTransform.x + (aimdirection.x / 4), .y = playerTransform.y - (aimdirection.y / 4), .z = 5, .xScale = 5, .yScale = 5 });
						ecs.addComponent(bullet, Sprite{ &texture3 });
						ecs.addComponent(bullet, Rigidbody{ .velocity = Vector2(aimdirection.x * 50, -aimdirection.y * 50), .drag = 0, .elasticity = 0, .kinematic = true });
						ecs.addComponent(bullet, BoxCollider{ .isTrigger = true });
						ecs.addComponent(bullet, Animator{});
						AnimationSystem::AddAnimation(bullet, explosion, "explosion");

						bullets.emplace(bullet);
						fireCooldown = 0.5f;
						//shooting sound
						mySpeaker2.Play(sound2);
					}
				}
			}
			else
			{
				ecs.getComponent<Sprite>(crosshair).enabled = false;
			}

			for (Entity bullet : bullets)
			{
				if (!ecs.entityExists(bullet))
					continue;

				BoxCollider hit;
				if (ecs.hasComponent<BoxCollider>(bullet))
					hit = ecs.getComponent<BoxCollider>(bullet);
				auto& rb = ecs.getComponent<Rigidbody>(bullet);
				auto& tf = ecs.getComponent<Transform>(bullet);
				auto& animator = ecs.getComponent<Animator>(bullet);

				if (!ecs.hasComponent<BoxCollider>(bullet))
				{
					if (!animator.playingAnimation)
					{
						bullets.erase(bullets.find(bullet));
						ecs.destroyEntity(bullet);
					}
					break;
				}

				for (const Collision& collision : hit.collisions)
				{
					if (collision.a == player || collision.b == player)
						continue;

					if (collision.type == Collision::Type::entityTrigger && collision.b != bullet)
					{
						rb.velocity = Vector2(0, 0);
						tf.xScale = 20;
						tf.yScale = 20;
						ecs.destroyEntity(collision.b);
						ecs.removeComponent<BoxCollider>(bullet);
						AnimationSystem::PlayAnimation(bullet, "explosion");

						break;
					}

					if (collision.type == Collision::Type::tilemapTrigger)
					{
						rb.velocity = Vector2(0, 0);
						tf.xScale = 20;
						tf.yScale = 20;
						ecs.removeComponent<BoxCollider>(bullet);
						AnimationSystem::PlayAnimation(bullet, "explosion");
						break;
					}
				}
			}
		}
		engine.soundDevice->SetSourceLocation(2, playerTransform.x, playerTransform.y, playerTransform.z);

		TransformSystem::SetPosition(playerTurret, Vector3(playerTransform.x, playerTransform.y, 1.6));

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		engine.soundDevice->SetLocation(playerTransform.x, playerTransform.y, playerTransform.z);
		engine.soundDevice->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

		//engine sound
		engine.soundDevice->SetSourceLocation(1, playerTransform.x, playerTransform.y, playerTransform.z);

		fireCooldown -= engine.deltaTime;

		turretController->Update(engine.deltaTime);
		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		cam.SetPosition(playerTransform.x, playerTransform.y, 100);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
//#endif


