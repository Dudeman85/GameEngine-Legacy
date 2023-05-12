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
	

	//Create a new shader by loading the fragment and vertex shaders from file
	//Leaving either of these empty will use the default shader for that type
	Shader shader = Shader("assets/vertexShader.glsl", "assets/fragmentShader.glsl");


	//Create a new entity
	Entity sprite = ecs.newEntity();
	
	ecs.addComponent(sprite, Transform{ .x = 300, .y = 200, .xScale = 20, .yScale = 20 });
	
	

	//Add the sprite component with a texture and custom shader
	Sprite& spriteComponent = ecs.addComponent(sprite, Sprite{});
	ecs.addComponent(sprite, Animator{});
	



	//Create animations from a spritesheet where one animation contains all the textures from one row of the spritesheet
	//The parameters are: the path to the image, how many sprites wide, how many sprites tall, and a vector of delays for each frame
	std::vector<Animation> anims = AnimationsFromSpritesheet("assets/warriorsheet.png", 8, 4, vector<int>(32, 250));
	
	//Add the first animation, aka the first row, created from the spritesheet
	//This has essentially the same results as the above example
	AnimationSystem::AddAnimation(sprite, anims[0], "Animation 1");
/*
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
	AnimationSystem::AddAnimation(sprite, anims, "Animation 1");
	engine.animationSystem->AddAnimation(sprite, anims, "Animation 1");
*/





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