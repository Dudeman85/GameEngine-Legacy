#include <engine/Application.h>

using namespace std;
using namespace engine;
ECS ecs;
int main()
{
	//Create the window and OpenGL context before creating EngineLib
	//Values define window size(x,y) and name
	GLFWwindow* window = CreateWindow(800, 600, "Window");
	//Initialize the default engine library
	EngineLib engine;
	//Create the camera
	Camera cam = Camera(800, 600);
	//changes window backround color, values 0-1, RGB
	RenderSystem::SetBackgroundColor(.6, .5, .1);

	//Load a new image
	Image testImage("assets/image.png");
	//Get the pxel at position (x, y)
	Pixel pixel = testImage[10][10];

	//Pixel members
	pixel.r;
	pixel.g;
	pixel.b;
	pixel.a;
	//Get a slice of testImage from (x1, y1) to (x2, y2). Both are inclusive
	Image slice = testImage.Slice(0, 0, 10, 10);


	//Load a texture from file
	Texture texture("assets/gradient.png", GL_NEAREST);
	//Create a texture from an image
	Texture texture2(testImage, GL_LINEAR);

	//Create a new shader by loading the fragment and vertex shaders from file
	//Leaving either of these empty will use the default shader for that type
	Shader shader = Shader("assets/vertexShader.glsl", "assets/fragmentShader.glsl");


	//Create a new entity
	Entity sprite = ecs.newEntity();
	//Add the sprite component with a texture and custom shader
	ecs.addComponent(sprite, Sprite{ &texture, &shader });
	//Add the component to an entity
	Entity player = ecs.newEntity();
	struct Position
	{
		float x, y, z;
	};

	//Register custom component
	ecs.registerComponent<Position>();

	ecs.addComponent(player, Position{ .x = 10.0f, .y = 25.25f, .z = 0.0f });
	//Get the Position component of player
	Position& position = ecs.getComponent<Position>(player);




	while (!glfwWindowShouldClose(window))
	{
		//
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();


	return 0;
}