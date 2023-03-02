#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "MusicBuffer.h"
#include "SoundSource.h"
#include <iostream>
/*
int main()
{
	std::cout << "starting...\n";

	SoundDevice* mysounddevice = SoundDevice::get();
	uint32_t sound1 = SoundBuffer::get()->addSoundEffect("assets/jump.wav");
	

	SoundSource mySpeaker;

	mySpeaker.Play(sound1);

	MusicBuffer myMusic("assets/spring-weather-1.wav");
	myMusic.Play();

	while (1)
	{
		myMusic.updateBufferStream();
	}

	std::cout << "got here\n";

	return 0;
}*/
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <cmath>

#include <engine/SpriteGL.h>


using namespace engine;

ECS ecs;

Camera cam = Camera(800, 600);

//Resize window callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	


	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam.Translate(1, 0);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam.Translate(-1, 0);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam.Translate(0, 1);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cam.Translate(0, -1);
		
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		cam.Translate(0, 0, -1);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		cam.Translate(0, 0, 1);
}

int main()
{
	//Initialize GLFW and set it to require OpenGL 3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	SoundDevice* mysounddevice = SoundDevice::get();
	//uint32_t sound1 = SoundBuffer::get()->addSoundEffect("assets/jump.wav");


	SoundSource mySpeaker;

	uint32_t sound1 = SoundBuffer::get()->addSoundEffect("assets/jump.wav");
	uint32_t sound2 = SoundBuffer::get()->addSoundEffect("assets/Jingle_Win_00.wav");
	MusicBuffer myMusic("assets/forest.wav");
	

	//Create window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Set the resize window callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glViewport(0, 0, 800, 600);

	//Load the shaders from files and use the compiled shader program
	//Shader shader("VertexShader.glsl", "FragmentShader.glsl");
	//shader.use();

	Texture texture = Texture("strawberry.png");

	ecs.registerComponent<Transform>();
	ecs.registerComponent<Sprite>();

	//Register the gravity system, it is accessible by this pointer
	std::shared_ptr<RenderSystem> renderSystem = ecs.registerSystem<RenderSystem>();

	//Add Position and Gravity components as requirements for GravitySys-tem
	Signature signature;
	signature.set(ecs.getComponentId<Transform>());
	signature.set(ecs.getComponentId<Sprite>());
	ecs.setSystemSignature<RenderSystem>(signature);

	renderSystem->SetBackgroundColor(0, .5, .1);

	//Create a new entity
	Entity sprite = ecs.newEntity();
	//Add the position component and set it's starting position
	ecs.addComponent(sprite, Transform{ .x = 0, .y = 0, .xScale = 50, .yScale = 50 });
	//Add the gravity component and set it's direction
	ecs.addComponent(sprite, Sprite{ .texture = &texture });

	//Create a new entity
	Entity sprite2 = ecs.newEntity();
	//Add the position component and set it's starting position
	ecs.addComponent(sprite2, Transform{ .x = 200, .y = 100, .xScale = 50, .yScale = 50 });
	//Add the gravity component and set it's direction
	ecs.addComponent(sprite2, Sprite{ .texture = &texture });


	myMusic.Play();
		
	


	//Main Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		renderSystem->Update(&cam);

		myMusic.updateBufferStream();
		
			

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			mySpeaker.Play(sound1);
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			mySpeaker.Play(sound2);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();

	return 0;
}