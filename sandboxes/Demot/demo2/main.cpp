//#if 0
#include <engine/Application.h>
#include <engine/Tilemap.h>
#include <vector>
#include <chrono>
#include <thread>
#include "PlayerController.h"
using namespace std;
using namespace engine;
// OpenGameArt
// itchio
ECS ecs;

int main()
{

	//Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(800, 600, "Window");


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
	
	engine.physicsSystem->gravity = Vector2(0, 0);
	engine.physicsSystem->step = 8;

	//Create the camera
	Camera cam = Camera(800, 600);

	Texture texture = Texture("assets/Gun_01.png");
	Texture texture2 = Texture("assets/crosshairEdit.png");
	Texture texture3 = Texture("assets/bullet.png");
	Texture texture4 = Texture("assets/Hull.png");

	//Create a new entity
	Entity player = ecs.newEntity();
	Transform& playerTransform = ecs.addComponent(player, Transform{ .x = 325, .y = -305, .z = 1, .xScale = 30, .yScale = 15 });
	ecs.addComponent(player, Sprite{&texture});
	//ecs.addComponent(player, Animator{});
	ecs.addComponent(player, Player{});


	Entity hull = ecs.newEntity();
	Transform& hullTransform = ecs.addComponent(player, Transform{ .x = 325, .y = -305, .z = 0, .xScale = 30, .yScale = 15 });
	ecs.addComponent(player, Sprite{ &texture4 });
	//ecs.addComponent(player, Animator{});
	ecs.addComponent(hull, Rigidbody{ .gravityScale = 1, .drag = 0, .friction = 0.0, .elasticity = 0 });
	ecs.addComponent(hull, BoxCollider{});
	ecs.addComponent(hull, Player{});
	BoxCollider& hullCollider = ecs.getComponent<BoxCollider>(hull);
	Rigidbody& hullRigidbody = ecs.getComponent<Rigidbody>(hull);

	// create entity crosshair for gamepad
	Entity crosshair = ecs.newEntity();
	// adds crosshair texture
	Transform crosshairTransform = ecs.addComponent(crosshair, Transform{ .x = 500, .y = 500, .xScale = 20, .yScale = 20 });
	ecs.addComponent(crosshair, Sprite{ &texture2 });

	vector <Entity>bullets;


	float fireCooldown = 0.04f;
	bool canFire = true;

	RenderSystem::SetBackgroundColor(0.3f, 0.3f, 0.1f);
	Tilemap map(&cam);
	map.loadMap("assets/demo2.tmx");
// TODO! Fixing tilemap position movement. Collider does not move with Tilemap.
	//map.position = glm::vec3(500.0f, 0.0f, 0.0f);


	engine.physicsSystem->SetTilemap(&map);
	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		
		
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		playerController->Update(window, engine.deltaTime, engine.physicsSystem);

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////OHJAINSÄÄDÖT////////////////////////////////////////////////////
		int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
		if (present == GLFW_TRUE) {

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
				engine.physicsSystem->Move(hull, movement * engine.deltaTime);

			}

			// sets crosshair position to zero
			Vector2 crosshairPosition(0, 0);
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
				Vector3 crosshairPosition = playerPosition + Vector3(aimdirection.x, -aimdirection.y, 0);
				TransformSystem::SetPosition(crosshair, crosshairPosition);

				float turretRotation(playerTransform.zRotation = atan2f(-rightThumbstick.y, rightThumbstick.x)*180 /3.14f);

				if (fireCooldown <= 0)
				{
					if (axes[5] > 0.5f)
					{

						Entity bullet = ecs.newEntity();
						ecs.addComponent(bullet, Transform{ .x = playerTransform.x + (aimdirection.x / 4), .y = playerTransform.y - (aimdirection.y / 4), .z = 1, .xScale = 5, .yScale = 5 });
						ecs.addComponent(bullet, Sprite{ &texture3 });
						ecs.addComponent(bullet, Rigidbody{ .velocity = Vector2(aimdirection.x * 50, -aimdirection.y * 50), .drag = 0, .elasticity = 0, .kinematic = true });
						ecs.addComponent(bullet, BoxCollider{ .isTrigger = true });
						bullets.push_back(bullet);
						fireCooldown = 0.4f;
					}
				}
				

				// set camera location between player and crosshair
				//cam.SetPosition(playerTransform.x + (aimdirection.x / 8), playerTransform.y - (aimdirection.y / 8), playerTransform.z);
			}
			else
			{
				ecs.getComponent<Sprite>(crosshair).enabled = false;
			}
			
			for (const Entity& bullet : bullets)
			{
				auto hit = ecs.getComponent<BoxCollider>(bullet);
				for (const Collision& collision : hit.collisions)
				{
					if (collision.type == Collision::Type::entityTrigger && collision.b != bullet)
					{
						ecs.destroyEntity(collision.b);
						bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
						ecs.destroyEntity(bullet);
					}
				}
			}

			if (buttons[0] == GLFW_PRESS)
			{
				//tapahtuu jotain
			}

		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		fireCooldown -= engine.deltaTime;
		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		
		map.draw();
		cam.SetPosition(playerTransform.x, playerTransform.y, +100);

		engine.Update(&cam);

		//TODO add view matrix and get projection matrix from camera so that tilemap is rendered in the correct place
		map.draw();

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
//#endif


