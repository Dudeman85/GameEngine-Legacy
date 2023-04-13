#include <engine/AL/SoundDevice.h>
#include <engine/AL/SoundBuffer.h>
#include <engine/AL/MusicBuffer.h>
#include <engine/AL/SoundSource.h>
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

	//engine.physicsSystem->Init(0, -10);

	//Create the camera
	Camera cam = Camera(800, 600);

	float volume = 0.5f;

	
	static SoundSource mySpeaker1;
	static SoundSource mySpeaker2;
	static SoundSource mySpeaker3;
	static SoundSource mySpeaker4;
	
	uint32_t sound1 = SoundBuffer::getFile()->addSoundEffect("assets/jump.wav");
	uint32_t sound2 = SoundBuffer::getFile()->addSoundEffect("assets/sound100.wav");
	uint32_t sound3 = SoundBuffer::getFile()->addSoundEffect("assets/sound100.wav");
	MusicBuffer myMusic("assets/forest.wav");
	myMusic.SetVolume(0.5f);
	mySpeaker1.setInverseDistanceClamped(1, 1.f, 100.f, 600.f, 2.5f);
	mySpeaker2.setInverseDistanceClamped(2, 1.f, 20.f, 200.f, 2.5f);
	//mySpeaker3.setInverseDistanceClamped(3, 1.f, 20.f, 200.f, 0.4f);
	//mySpeaker4.setInverseDistanceClamped(4, 1.f, 20.f, 200.f, 0.4f);
	//Load a new texture
	Texture texture = Texture("assets/strawberry.png");

	//Create a new entity
	Entity player = ecs.newEntity();
	ecs.addComponent(player, Transform{ .x = 0, .y = 0, .xScale = 20, .yScale = 20 });
	ecs.addComponent(player, Sprite{});
	ecs.addComponent(player, Animator{});

	//Define the test animation
	Animator animator = ecs.getComponent<Animator>(player);
	auto testAnims = AnimationsFromSpritesheet("assets/gradient.png", 2, 2, vector<int>(4, 200));
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
		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		{
			volume -= 0.01f;
			if (volume < 0.0f) volume = 0.0f; // Clamp the volume to a minimum of 0.0f
			myMusic.SetVolume(volume);
		}
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			volume += 0.01f;
			if (volume > 1.0f) volume = 1.0f; // Clamp the volume to a max 1.0f
			myMusic.SetVolume(volume);
		}

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
		engine.soundDevice->SetLocation(playerTransform.x, playerTransform.y, playerTransform.z);
		engine.soundDevice->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
		
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			Transform sprite2Transform = ecs.getComponent<Transform>(sprite2);
			mySpeaker1.Play(sound2);
			engine.soundDevice->SetSourceLocation(1, sprite2Transform.x, sprite2Transform.y, 20.f);
			/*Transform sprite3Transform = ecs.getComponent<Transform>(sprite3);
			sd->SetSourceLocation(3, sprite3Transform.x, sprite3Transform.y, 2.f);
			mySpeaker3.Play(sound2);*/
		}
		
		
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			Transform sprite4Transform = ecs.getComponent<Transform>(sprite4);
			mySpeaker2.Play(sound2);
			engine.soundDevice->SetSourceLocation(2, sprite4Transform.x, sprite4Transform.y, 20.f);
			
			/*Transform sprite5Transform = ecs.getComponent<Transform>(sprite5);
			sd->SetSourceLocation(4, sprite5Transform.x, sprite5Transform.y, 0.f);
			mySpeaker4.Play(sound2);*/
		}
		//Update all engine systems
		engine.Update(&cam);
		


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}