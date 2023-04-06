#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "MusicBuffer.h"
#include "SoundSource.h"
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <cmath>

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

	SoundDevice* sd1 = SoundDevice::getDevice()->getDevice();
	SoundDevice* sd2 = SoundDevice::getDevice()->getDevice();
	SoundDevice* sd3 = SoundDevice::getDevice()->getDevice();
	static SoundSource mySpeaker1;
	uint32_t sound1 = SoundBuffer::getFile()->addSoundEffect("assets/jump.wav");
	static SoundSource mySpeaker2;
	uint32_t sound2 = SoundBuffer::getFile()->addSoundEffect("assets/sound100.wav");
	static SoundSource mySpeaker3;
	uint32_t sound3 = SoundBuffer::getFile()->addSoundEffect("assets/sound100.wav");
	MusicBuffer myMusic("assets/forest.wav");
	
	alSourcef(1, AL_GAIN, 1.0f);
	alSourcef(1, AL_REFERENCE_DISTANCE, 20.f);
	alSourcef(1, AL_MAX_DISTANCE, 150.f);
	alSourcef(1, AL_ROLLOFF_FACTOR, 0.4f);

	alSourcef(2, AL_GAIN, 1.0f);
	alSourcef(2, AL_REFERENCE_DISTANCE, 10);
	alSourcef(2, AL_MAX_DISTANCE, 150.f);
	alSourcef(2, AL_ROLLOFF_FACTOR, 0.5f);

	alSourcef(3, AL_GAIN, 1.0f);
	alSourcef(3, AL_REFERENCE_DISTANCE, 20.f);
	alSourcef(3, AL_MAX_DISTANCE, 150.f);
	alSourcef(3, AL_ROLLOFF_FACTOR, 0.4f);
	alDistanceModel(AL_INVERSE_DISTANCE);
	


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
	ecs.addComponent(sprite2, Sprite{ &texture });
	//Create a new entity
	Entity sprite3 = ecs.newEntity();
	ecs.addComponent(sprite3, Transform{ .x = -300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite3, Sprite{ &texture });
	//Create a new entity
	Entity sprite4 = ecs.newEntity();
	ecs.addComponent(sprite4, Transform{ .x = -300, .y = 200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite4, Sprite{ &texture });
	//Create a new entity
	Entity sprite5 = ecs.newEntity();
	ecs.addComponent(sprite5, Transform{ .x = 300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite5, Sprite{ &texture });

	engine.renderSystem->SetBackgroundColor(0, .5, .1);

	myMusic.Play();

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		myMusic.updateBufferStream();
		

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			myMusic.Pause();
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			myMusic.Resume();
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
		sd1->SetLocation(playerTransform.x, playerTransform.y, playerTransform.z);
		sd1->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
		

		
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			Transform sprite2Transform = ecs.getComponent<Transform>(sprite2);
			mySpeaker1.Play(sound2);
			sd2->SetSourceLocation(1, sprite2Transform.x, sprite2Transform.y, 0.f);
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			Transform sprite4Transform = ecs.getComponent<Transform>(sprite4);
			mySpeaker2.Play(sound2);
			sd3->SetSourceLocation(3, sprite4Transform.x, sprite4Transform.y, 0.f);
		}
		//Update all engine systems
		engine.Update(&cam);
		


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}