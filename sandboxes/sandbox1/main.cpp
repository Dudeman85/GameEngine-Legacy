#include <iostream>
#include <engine/Tilemap.h>

#include <chrono> // std::chrono::microseconds
#include <thread> // std::this_thread::sleep_for

#include <fstream>

#include <engine/Application.h>
#include <../src/detail/pugixml.hpp>

using namespace std;
using namespace engine;
ECS ecs;

//void mouse_callback(GLFWwindow * window, double xpos, double ypos)
//	{
//	cout << xpos << "x : " << ypos <<"y"<< endl;
//	}

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(800, 600, "Window");

	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);


	//glfwSetCursorPosCallback(window, mouse_callback);

	//Initialize the default engine library
	EngineLib engine;

	engine.physicsSystem->gravity = Vector2(0, -400);
	engine.physicsSystem->step = 4;

	//Create the camera
	Camera cam = Camera(800, 600);

	float volume = 0.2f;


	static SoundSource mySpeaker1;
	static SoundSource mySpeaker2;
	static SoundSource mySpeaker3;
	static SoundSource mySpeaker4;

	uint32_t sound1 = SoundBuffer::getFile()->addSoundEffect("assets/jump.wav");
	uint32_t sound2 = SoundBuffer::getFile()->addSoundEffect("assets/sound100.wav");

	MusicBuffer myMusic("assets/forest.wav");
	myMusic.SetVolume(0.2f);
	mySpeaker1.setLinearDistanceClamped(1, 1.f, 100.f, 600.f, 1.f);
	mySpeaker2.setLinearDistanceClamped(2, 1.f, 20.f, 200.f, 1.f);
	mySpeaker3.setLinearDistanceClamped(3, 1.f, 20.f, 400.f, 2.4f);
	mySpeaker4.setLinearDistanceClamped(4, 1.f, 20.f, 300.f, 1.f);
	//Load a new texture
	Texture texture = Texture("assets/strawberry.png");
	Texture texture2 = Texture("assets/crosshairEdit.png");



	GLFWimage cursor_image;
	cursor_image.pixels = stbi_load("assets/crosshairEdit.png", &cursor_image.width, &cursor_image.height, 0, 4); // 4 tarkoittaa, että kuvassa on 4 kanavaa (RGBA)
	GLFWcursor* cursor = glfwCreateCursor(&cursor_image, 0, 0);
	stbi_image_free(cursor_image.pixels);
	glfwSetCursor(window, cursor);



	//Create a new entity
	Entity player = ecs.newEntity();
	ecs.addComponent(player, Transform{ .x = 0, .y = 0, .xScale = 20, .yScale = 20 });
	ecs.addComponent(player, Sprite{});
	ecs.addComponent(player, Animator{});
	ecs.addComponent(player, Rigidbody{ .gravityScale = 0, .drag = 0,  .friction = 0, .elasticity = 0 });
	ecs.addComponent(player, BoxCollider{});

	//Define the test animation
	Animator animator = ecs.getComponent<Animator>(player);
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
	//Top-Left
	Entity sprite4 = ecs.newEntity();
	ecs.addComponent(sprite4, Transform{ .x = -310, .y = 200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite4, Sprite{ &texture });
	ecs.addComponent(sprite4, Rigidbody{ .drag = 0.1, .friction = 0.2, .elasticity = 0.125, .kinematic = false });
	ecs.addComponent(sprite4, BoxCollider{});
	//Bottom-Right
	Entity sprite5 = ecs.newEntity();
	ecs.addComponent(sprite5, Transform{ .x = 300, .y = -200, .xScale = 20, .yScale = 20 });
	ecs.addComponent(sprite5, Sprite{ &texture });
	ecs.addComponent(sprite5, Rigidbody{ .velocity = Vector2(-985, 1000), .drag = 0.25, .elasticity = 0.125, .kinematic = false });
	ecs.addComponent(sprite5, BoxCollider{});


	// Luo uusi entity crosshair
	Entity crosshair = ecs.newEntity();



	/*
	GLFWcursor* cursor = glfwCreateCursor((GLFWimage*)&cursorTextureID, 20, 20);
	glfwSetCursor(window, cursor);*/



	RenderSystem::SetBackgroundColor(0, .5, .1);

	BoxCollider& collider = ecs.getComponent<BoxCollider>(player);
	myMusic.Play();

	//play sound files
	mySpeaker4.Play(sound1);
	//sets sound to loop, value 1=true
	mySpeaker4.SetLooping(1);

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{

		myMusic.updateBufferStream();


		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			myMusic.Pause();
			mySpeaker4.Pause();
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			myMusic.Resume();
			mySpeaker4.Resume();
		}
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

		//test movement
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

		engine.soundDevice->SetLocation(playerTransform.x, playerTransform.y, playerTransform.z);
		engine.soundDevice->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

		if (collider.collisions.size() > 0)
		{
			for (Collision c : collider.collisions)
			{
				cout << c.a << " " << c.b << endl;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			Transform sprite2Transform = ecs.getComponent<Transform>(sprite2);
			mySpeaker1.Play(sound2);
			engine.soundDevice->SetSourceLocation(1, sprite2Transform.x, sprite2Transform.y, 20.f);
			mySpeaker1.SetLooping(1);
			Transform sprite3Transform = ecs.getComponent<Transform>(sprite3);
			engine.soundDevice->SetSourceLocation(3, sprite3Transform.x, sprite3Transform.y, 2.f);
			mySpeaker3.Play(sound2);
		}

		Transform sprite5Transform = ecs.getComponent<Transform>(sprite5);
		engine.soundDevice->SetSourceLocation(4, sprite5Transform.x, sprite5Transform.y, 0.f);


		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		{
			Transform sprite4Transform = ecs.getComponent<Transform>(sprite4);
			mySpeaker2.Play(sound2);
			engine.soundDevice->SetSourceLocation(2, sprite4Transform.x, sprite4Transform.y, 20.f);
		}


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			Transform sprite4Transform = ecs.getComponent<Transform>(sprite4);
			mySpeaker2.Play(sound2);
			engine.soundDevice->SetSourceLocation(2, sprite4Transform.x, sprite4Transform.y, 20.f);
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////OHJAINSÄÄDÖT////////////////////////////////////////////////////

		GLFWgamepadstate state;

		// set player speed
		float player_speed = 500.0f;
		//define deadzone
		float deadzone_size = 0.2f;
		//calculate deadzone barrier
		float deadzone_treshold = deadzone_size / 2.0f;
		//Camera max distance
		float max_distance = 30.0f;

		// Hae peliohjaimen vasemman analogisen tikun tila
		int axis_count;
		int button_count;
		int count;
		float radian = 5.0f;

		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axis_count);
		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &button_count);


		if (abs(axes[0]) > deadzone_treshold || abs(axes[1]) > deadzone_treshold) {
			// Aseta liikkumisvektori pelaajan nopeuden perusteella
			Vector2 movement(axes[0] * player_speed, -axes[1] * player_speed);

			// Liikuta pelaajaa käyttämällä liikkumisvektoria
			engine.physicsSystem->Move(player, movement * engine.deltaTime);

		}


		Vector2 crosshairPosition(300, -200);
		if (axes[2] > deadzone_treshold || axes[3] > deadzone_treshold ||
			axes[2] < -deadzone_treshold || axes[3] < -deadzone_treshold) {

			// Muuta tatin koordinaatiston origo keskipisteeksi ja skaalaa se välille [-1, 1]
			Vector2 right_thumbstick(axes[2], axes[3]);
			right_thumbstick = (right_thumbstick + Vector2(1.0f, 1.0f)) / 2.0f;
			right_thumbstick -= Vector2(0.5f, 0.5f);
			right_thumbstick *= 2.0f;

			// Laske uusi sijainti
			crosshairPosition.x += right_thumbstick.x * 100.0f;
			crosshairPosition.y -= right_thumbstick.y * 100.0f;

			// Luo vektori, joka osoittaa tatin suuntaan
			float angle = atan2f(right_thumbstick.y, right_thumbstick.x);
			Vector2 direction = Vector2(cosf(angle), sinf(angle));

			// Normalisoi vektori ja kerro se etäisyydellä 100
			direction.Normalize();
			direction *= 150.0f;

			// Lisää pelaajan sijainti crosshairin sijaintiin
			Vector3 player_position(playerTransform.x, playerTransform.y, playerTransform.z);
			Vector3 crosshair_position = player_position + Vector3(direction.x, -direction.y, 0);

			// Lisää transform- ja sprite-komponentit crosshairiin
			ecs.addComponent(crosshair, Transform{ .x = crosshair_position.x, .y = crosshair_position.y, .xScale = 20, .yScale = 20 });
			ecs.addComponent(crosshair, Sprite{ &texture2 });
			cam.SetPosition(playerTransform.x + (crosshair_position.x / 4), playerTransform.y + (crosshair_position.y / 4), playerTransform.z);
		}

		if (buttons[0] == GLFW_PRESS) {
			Vector2 movement(0.0f, player_speed);
			engine.physicsSystem->Move(player, movement * engine.deltaTime);
		}
		if (buttons[1] == GLFW_PRESS) {
			Transform sprite4Transform = ecs.getComponent<Transform>(sprite4);
			mySpeaker2.Play(sound2);
			engine.soundDevice->SetSourceLocation(2, sprite4Transform.x, sprite4Transform.y, 20.f);
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////////////




		//Update all engine systems
		engine.Update(&cam);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
