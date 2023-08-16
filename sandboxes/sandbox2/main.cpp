#include <iostream>
#include <engine/Application.h>
#include <engine/Tilemap.h>
#include <vector>
#include <map>
#include <chrono>
#include <thread>

// Freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace engine;

ECS ecs;


int main()
{
	// Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(800, 600, "Window");

	// Create the Camera
	Camera cam = Camera(800, 600);

	// Initialize the default engine library
	EngineLib engine;

	// Physics system
	engine.physicsSystem->gravity = Vector2(0, -400);
	engine.physicsSystem->step = 4;

	// Load a new Texture
	Texture texture = Texture("assets/strawberry.png");

	// Create a new entity
	Entity player = ecs.newEntity();
	ecs.addComponent(player, Transform{ .x = 0, .y = 0 ,.z = 1.5, .xScale = 20, .yScale = 20 });
	ecs.addComponent(player, Sprite{});
	ecs.addComponent(player, Animator{});
	ecs.addComponent(player, Rigidbody{ .gravityScale = 0, .drag = 0, .friction = 0, .elasticity = 0 });
	ecs.addComponent(player, BoxCollider{});
	BoxCollider& collider = ecs.getComponent<BoxCollider>(player);

	// Define the test animation
	Animator& animator = ecs.getComponent<Animator>(player);
	auto testAnims = AnimationsFromSpritesheet("assets/gradient.png", 2, 2, vector<int>(4, 200));
	AnimationSystem::AddAnimation(player, testAnims[0], "1");
	AnimationSystem::AddAnimation(player, testAnims[1], "2");
	AnimationSystem::PlayAnimation(player, "2", true);

	// Top-Right
	Entity sprite2 = ecs.newEntity();
	ecs.addComponent(sprite2, Transform{ .x = 300, .y = 200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite2, Sprite{ &texture });
	ecs.addComponent(sprite2, Rigidbody{ .kinematic = true });
	ecs.addComponent(sprite2, BoxCollider{ .scale = Vector2(10, 1) });

	// Bottom-Left
	Entity sprite3 = ecs.newEntity();
	ecs.addComponent(sprite3, Transform{ .x = -300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite3, Sprite{ &texture });
	ecs.addComponent(sprite3, Rigidbody{ .kinematic = true });
	ecs.addComponent(sprite3, BoxCollider{});

	// Top-Left
	Entity sprite4 = ecs.newEntity();
	ecs.addComponent(sprite4, Transform{ .x = -310, .y = 200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite4, Sprite{ &texture });
	ecs.addComponent(sprite4, Rigidbody{ .drag = 0.1, .friction = 0.2, .elasticity = 0.125, .kinematic = false });
	ecs.addComponent(sprite4, BoxCollider{});

	// Bottom-Right
	Entity sprite5 = ecs.newEntity();
	ecs.addComponent(sprite5, Transform{ .x = 300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite5, Sprite{ &texture });
	ecs.addComponent(sprite5, Rigidbody{ .velocity = Vector2(-985, 1000), .drag = 0.25, .elasticity = 0.125, .kinematic = false });
	ecs.addComponent(sprite5, BoxCollider{});

	


	// Window background colour
	RenderSystem::SetBackgroundColor(68, 154, 141);

	// Tilemap
	Tilemap map(&cam);
	map.loadMap("assets/Tiled/Tilemaps/demo.tmx");
	engine.physicsSystem->SetTilemap(&map);
	engine.renderSystem->SetTilemap(&map);
	map.position = glm::vec3(0.0f, 0.0f, 0.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}

		// Test movement
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			engine.physicsSystem->Move(player, Vector2(500, 0) * engine.deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			engine.physicsSystem->Move(player, Vector2(-500, 0) * engine.deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			engine.physicsSystem->Move(player, Vector2(0, 500) * engine.deltaTime);
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			engine.physicsSystem->Move(player, Vector2(0, -500) * engine.deltaTime);
		}

		Transform playerTransform = ecs.getComponent<Transform>(player);
		cam.SetPosition(playerTransform.x, playerTransform.y, playerTransform.z + 100);

		if (collider.collisions.size() > 0)
		{
			for (const Collision& c : collider.collisions)
			{
				cout << c.a << " " << c.b << endl;
			}
		}

	//TODO add view matrix and get projection matrix from camera so that tilemap is rendered in the correct place
	//map.draw(1.0f);

		// Update all engine systems, this usually should go last in the game loop
		// For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		// Text Rendering functions
		//RenderText(m_shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		//RenderText(m_shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		// OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();

	return 0;
}