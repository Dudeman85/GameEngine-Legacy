#include <engine/Application.h>

using namespace std;
using namespace engine;

ECS ecs;

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(800, 600, "Window");

	//Initialize the default engine library
	EngineLib engine;

	engine.physicsSystem->Init(0, -10);

	//Create the camera
	Camera cam = Camera(800, 600);

	//Load a new texture
	Texture texture = Texture("strawberry.png");

	//Create a new entity
	Entity player = ecs.newEntity();
	ecs.addComponent(player, Transform{ .x = 0, .y = 0, .xScale = 20, .yScale = 20 });
	ecs.addComponent(player, Sprite{});
	ecs.addComponent(player, Animator{});

	//Define the test animation
	Animator animator = ecs.getComponent<Animator>(player);
	auto testAnims = AnimationsFromSpritesheet("gradient.png", 2, 2, vector<int>(4, 200));
	engine.animationSystem->AddAnimation(player, testAnims[0], "1");
	engine.animationSystem->AddAnimation(player, testAnims[1], "2");
	engine.animationSystem->PlayAnimation(player, "2", true);

	//Create a new entity
	Entity sprite2 = ecs.newEntity();
	ecs.addComponent(sprite2, Transform{ .x = 300, .y = 200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite2, Sprite{&texture});
	//Create a new entity
	Entity sprite3 = ecs.newEntity();
	ecs.addComponent(sprite3, Transform{ .x = -300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite3, Sprite{&texture});
	//Create a new entity
	Entity sprite4 = ecs.newEntity();
	ecs.addComponent(sprite4, Transform{ .x = -300, .y = 200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite4, Sprite{&texture});
	//Create a new entity
	Entity sprite5 = ecs.newEntity();
	ecs.addComponent(sprite5, Transform{ .x = 300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite5, Sprite{&texture});

	engine.renderSystem->SetBackgroundColor(0, .5, .1);

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			engine.transformSystem->Translate(player, 5, 0);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			engine.transformSystem->Translate(player, -5, 0);
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			engine.transformSystem->Translate(player, 0, 5);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			engine.transformSystem->Translate(player, 0, -5);

		Transform playerTransform = ecs.getComponent<Transform>(player);
		cam.SetPosition(playerTransform.x, playerTransform.y, playerTransform.z);

		//Update all engine systems
		engine.Update(&cam);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}