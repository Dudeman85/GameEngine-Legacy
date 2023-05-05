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

	engine.physicsSystem->gravity = Vector2(0, -6000);
	engine.physicsSystem->step = 4;

	//Create the camera
	Camera cam = Camera(800, 600);

	//Load a new texture
	Texture texture = Texture("assets/strawberry.png");

	//Create a new entity
	Entity player = ecs.newEntity();
	ecs.addComponent(player, Transform{ .x = 0, .y = 25, .xScale = 20, .yScale = 20 });
	ecs.addComponent(player, Sprite{});
	ecs.addComponent(player, Animator{});
	ecs.addComponent(player, Rigidbody{ .drag = 0, .gravityScale = 1, .friction = 0.2, .elasticity = 0 });
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
	ecs.addComponent(sprite2, Rigidbody{ .isStatic = true });
	ecs.addComponent(sprite2, BoxCollider{ .scale = Vector2(10, 1) });
	//Bottom-Left
	Entity sprite3 = ecs.newEntity();
	ecs.addComponent(sprite3, Transform{ .x = -300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite3, Sprite{ &texture });
	ecs.addComponent(sprite3, Rigidbody{ .isStatic = true });
	ecs.addComponent(sprite3, BoxCollider{});
	Entity sprite6 = ecs.newEntity();
	ecs.addComponent(sprite6, Transform{ .x = -320, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite6, Sprite{ &texture });
	ecs.addComponent(sprite6, Rigidbody{ .isStatic = true });
	ecs.addComponent(sprite6, BoxCollider{});
	//Top-Left
	Entity sprite4 = ecs.newEntity();
	ecs.addComponent(sprite4, Transform{ .x = -310, .y = 200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite4, Sprite{ &texture });
	ecs.addComponent(sprite4, Rigidbody{ .drag = 0.1, .gravityScale = 0, .friction = 0.2, .elasticity = 0.125, .isStatic = false });
	ecs.addComponent(sprite4, BoxCollider{});
	//Bottom-Right
	Entity sprite5 = ecs.newEntity();
	ecs.addComponent(sprite5, Transform{ .x = 300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite5, Sprite{ &texture });
	ecs.addComponent(sprite5, Rigidbody{ .velocity = Vector2(-985, 2000), .drag = 0.25, .elasticity = 0.625, .isStatic = false });
	ecs.addComponent(sprite5, BoxCollider{});

	RenderSystem::SetBackgroundColor(0, .5, .1);

	Tilemap map(&cam);
	map.loadMap("assets/demo.tmx");

	engine.physicsSystem->SetTilemap(&map);

	bool jumpHeld = false;

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds{100});

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//test movement
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			engine.physicsSystem->Move(player, Vector2(500, 0) * engine.deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			engine.physicsSystem->Move(player, Vector2(-500, 0) * engine.deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			engine.physicsSystem->Move(player, Vector2(0, -500) * engine.deltaTime);
		}

		//When jump is pressed
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			//If touching ground
			if (playerCollider.sidesCollided[Direction::down])
			{
				PhysicsSystem::Impulse(player, Vector2(0, 3200));
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
			if (jumpHeld)
			{
				//If not touching ground
				if (!playerCollider.sidesCollided[Direction::down])
				{
					playerRigidbody.velocity = Vector2(playerRigidbody.velocity.x, min(playerRigidbody.velocity.y, 700.0f));
				}
				jumpHeld = false;
			}
		}





		if (playerCollider.collisions.size() > 0)
		{
			for (const Collision& c : playerCollider.collisions)
			{
				if (c.type == Collision::Type::entity)
				{
					cout << "HERE\n";
				}

				if (c.side == Direction::down)
					cout << "Player touching ground\n";
			}
		}

		Transform playerTransform = ecs.getComponent<Transform>(player);
		cam.SetPosition(playerTransform.x, playerTransform.y, playerTransform.z);

		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		map.draw();

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}