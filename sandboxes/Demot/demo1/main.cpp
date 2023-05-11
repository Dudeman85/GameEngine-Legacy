#include <engine/Application.h>
#include <engine/Tilemap.h>

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

	engine.physicsSystem->gravity = Vector2(0, -10000);
	engine.physicsSystem->step = 4;

	//Create the camera
	Camera cam = Camera(800, 600);

	//Load a new texture
	Texture texture = Texture("assets/strawberry.png");

	//Create a new entity
	Entity player = ecs.newEntity();
	Transform& playerTransform = ecs.addComponent(player, Transform{ .x = 0, .y = 25, .xScale = 20, .yScale = 20 });
	ecs.addComponent(player, Sprite{});
	ecs.addComponent(player, Animator{});
	ecs.addComponent(player, Rigidbody{ .gravityScale = 1, .drag = 0, .friction = 0.0, .elasticity = 0 });
	ecs.addComponent(player, BoxCollider{});
	BoxCollider& playerCollider = ecs.getComponent<BoxCollider>(player);
	Rigidbody& playerRigidbody = ecs.getComponent<Rigidbody>(player);

	//Define the test animation
	Animator& animator = ecs.getComponent<Animator>(player);
	auto testAnims = AnimationsFromSpritesheet("assets/gradient.png", 2, 2, vector<int>(4, 200));
	AnimationSystem::AddAnimation(player, testAnims[0], "1");
	AnimationSystem::AddAnimation(player, testAnims[1], "2");
	AnimationSystem::PlayAnimation(player, "2", true);

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
	map.loadMap("assets/demo.tmx");

	engine.physicsSystem->SetTilemap(&map);

	bool jumpHeld = false;
	bool jumping = false;

	float maxSpeed = 2000;
	float accelSpeed = 400;
	float reactivityMult = 2;
	float maxJumpSpeed = 3800;
	float jumpAccelSpeed = 1000;

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds{100});

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//test movement
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			if (playerRigidbody.velocity.x < maxSpeed)
			{
				Vector2 impulse;

				//Decelerate faster
				if (playerRigidbody.velocity.x < 0)
					impulse = Vector2(min(accelSpeed * reactivityMult, maxSpeed - playerRigidbody.velocity.x), 0);
				else
					impulse = Vector2(min(accelSpeed, maxSpeed - playerRigidbody.velocity.x), 0);

				PhysicsSystem::Impulse(player, impulse);
			}
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			if (playerRigidbody.velocity.x > -maxSpeed)
			{
				Vector2 impulse;

				//Decelerate faster
				if (playerRigidbody.velocity.x > 0)
					impulse = Vector2(max(-accelSpeed * reactivityMult, -maxSpeed - playerRigidbody.velocity.x), 0);
				else
					impulse = Vector2(max(-accelSpeed, -maxSpeed - playerRigidbody.velocity.x), 0);

				PhysicsSystem::Impulse(player, impulse);
			}
		}
		else
		{
			//Slow the player down aka friction
			if (abs(playerRigidbody.velocity.x) > 0)
				playerRigidbody.velocity.x -= (playerRigidbody.velocity.x / abs(playerRigidbody.velocity.x) * min(accelSpeed, abs(playerRigidbody.velocity.x)));
		}

		//When jump is pressed
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			//Accelerate to max jump speed while holding jump
			if (jumping)
			{
				//If ceiling is hit cancel the jump
				if (playerCollider.sidesCollided[Direction::up])
					jumping = false;

				if (playerRigidbody.velocity.y + jumpAccelSpeed < maxJumpSpeed)
				{
					PhysicsSystem::Impulse(player, Vector2(0, jumpAccelSpeed));
				}
				else
				{
					playerRigidbody.velocity.y = maxJumpSpeed;
					jumping = false;
				}
			}

			//If touching ground
			if (playerCollider.sidesCollided[Direction::down])
			{
				jumping = true;
			}
			//If not touching ground
			else
			{
				//Walljump
				if (!jumpHeld)
				{
					//Right side
					if (playerCollider.sidesCollided[Direction::right])
					{
						PhysicsSystem::Impulse(player, Vector2(-3000, 4000));
						cout << "going left\n";
					}

					//Left side
					if (playerCollider.sidesCollided[Direction::left])
					{
						PhysicsSystem::Impulse(player, Vector2(3000, 4000));
						cout << "going right\n";
					}
				}
			}
			jumpHeld = true;
		}
		else
		{
			jumping = false;
			if (jumpHeld)
			{
				//If not touching ground
				if (!playerCollider.sidesCollided[Direction::down])
				{
					playerRigidbody.velocity = Vector2(playerRigidbody.velocity.x, min(playerRigidbody.velocity.y, 1500.0f));
				}
				jumpHeld = false;
			}
		}

		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);


		map.draw();
		cam.SetPosition(playerTransform.x, playerTransform.y, playerTransform.z);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}