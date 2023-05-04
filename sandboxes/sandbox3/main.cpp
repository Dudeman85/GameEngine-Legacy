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
	Image slice = testImage.Slice(0, 0, 30, 30);


	//Load a texture from file
	Texture texture("assets/gradient.png", GL_NEAREST);
	//Create a texture from an image
	Texture texture2(testImage, GL_LINEAR);

	//Create a new shader by loading the fragment and vertex shaders from file
	//Leaving either of these empty will use the default shader for that type
	Shader shader = Shader("assets/vertexShader.glsl", "assets/fragmentShader.glsl");


	//Create a new entity
	Entity sprite = ecs.newEntity();
	
	//
	//TÄMÄ PUUTTUU OHJEESTA
	ecs.addComponent(sprite, Transform{ .x = 300, .y = 200, .xScale = 20, .yScale = 20 });
	//
	

	//Add the sprite component with a texture and custom shader
	ecs.addComponent(sprite, Sprite{ &texture});
	//Add the component to an entity
	Entity player = ecs.newEntity();
	struct Position
	{
		float x, y, z;
	};


	//Create multiple textures from a spritesheet.
	vector<Texture*> slicedTextures = SliceSpritesheet("spritesheet.png", 4, 6);
	//Copy the first row of the spritesheet to a new vector
	vector<Texture*> textures(4);
	copy(slicedTextures.begin(), slicedTextures.begin() + 4, textures.begin());
	//Create a vector of delays with each delay being 250ms
	vector<int> delays(4, 250);

	//Create the animation
	Animation anim = Animation(textures, delays);

	//Add the Animator component to the sprite entity
	ecs.addComponent(sprite, Animator{});

	//Add the anim animation with the name "Animation 1" to the sprite entity
	AnimationSystem::AddAnimation(sprite, anim, "Animation 1");


	//Create animations from a spritesheet where one animation contains all the textures from one row of the spritesheet
	//The parameters are: the path to the image, how many sprites wide, how many sprites tall, and a vector of delays for each frame
	std::vector<Animation> anims = AnimationsFromSpritesheet("spritesheet.png", 4, 6, vector<int>(24, 250));

	//Add the first animation, aka the first row, created from the spritesheet
	//This has essentially the same results as the above example
	AnimationSystem::AddAnimation(sprite, anims[0], "Animation 2");

	//Start playing an animation named "Animation 1", without looping
	AnimationSystem::PlayAnimation(sprite, "Animation 1", false);
	//Stop playing the currently playing animation
	AnimationSystem::StopAnimation(sprite);

	//Get the sprite entity's Animator component
	Animator animator = ecs.getComponent<Animator>(sprite);
	//The animator component has members which can be used to get its current state.
	//These should not be directly modified unless you know what you're doing. 
	//For example if you wanted to temporarily pause the animation you could change playingAnimation to false.
	animator.currentAnimation;
	animator.animationFrame;
	animator.repeatAnimation;
	animator.playingAnimation;
	animator.animationTimer;

	//These are equivalent
	AnimationSystem::AddAnimation(sprite, anim, "Animation 1");
	engine.animationSystem->AddAnimation(sprite, anim, "Animation 1");





	//Register custom component
	ecs.registerComponent<Position>();

	ecs.addComponent(player, Position{ .x = 10.0f, .y = 25.25f, .z = 0.0f });
	//Get the Position component of player
	Position& position = ecs.getComponent<Position>(player);
	RenderSystem::SetBackgroundColor(.6, .5, .1);



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