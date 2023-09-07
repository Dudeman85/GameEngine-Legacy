//#include <iostream>
//#include <engine/Tilemap.h>
//
//#include <chrono> // std::chrono::microseconds
//#include <thread> // std::this_thread::sleep_for
//
//#include <fstream>
//
//#include <engine/Application.h>
//#include <../src/detail/pugixml.hpp>
//
//using namespace std;
//using namespace engine;
//ECS ecs;
//
////void mouse_callback(GLFWwindow * window, double xpos, double ypos)
////	{
////	cout << xpos << "x : " << ypos <<"y"<< endl;
////	}
//
//int main()
//{
//	//Create the window and OpenGL context before creating EngineLib
//	GLFWwindow* window = CreateWindow(800, 600, "Window");
//
//	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
//
//
//	//glfwSetCursorPosCallback(window, mouse_callback);
//
//	//Initialize the default engine library
//	EngineLib engine;
//
//	//engine.physicsSystem->gravity = Vector2(0, 0);
//	//engine.physicsSystem->step = 4;
//
//	//Create the camera
//	Camera cam = Camera(800, 600);
//
//	float volume = 0.2f;
//
//
//	static SoundSource mySpeaker1;
//	static SoundSource mySpeaker2;
//	static SoundSource mySpeaker3;
//	static SoundSource mySpeaker4;
//
//	uint32_t sound1 = SoundBuffer::getFile()->addSoundEffect("assets/jump.wav");
//	uint32_t sound2 = SoundBuffer::getFile()->addSoundEffect("assets/sound100.wav");
//	uint32_t sound3 = SoundBuffer::getFile()->addSoundEffect("assets/bang_09.wav");
//	uint32_t sound4 = SoundBuffer::getFile()->addSoundEffect("assets/hit.wav");
//
//	MusicBuffer myMusic("assets/forest.wav");
//	myMusic.SetVolume(0.2f);
//	mySpeaker1.setLinearDistanceClamped(1.f, 100.f, 600.f, 1.f);
//	mySpeaker2.setLinearDistanceClamped(1.f, 20.f, 200.f, 1.f);
//	mySpeaker3.setLinearDistanceClamped(1.f, 20.f, 400.f, 2.4f);
//	mySpeaker4.setLinearDistanceClamped(1.f, 20.f, 300.f, 1.f);
//	//Load a new texture
//	Texture texture = Texture("assets/strawberry.png");
//	Texture texture2 = Texture("assets/crosshairEdit.png");
//	Texture texture3 = Texture("assets/bullet.png");
//
//
//	// mouse settings
//	GLFWimage cursor_image;
//	cursor_image.pixels = stbi_load("assets/crosshairEdit.png", &cursor_image.width, &cursor_image.height, 0, 4);
//	GLFWcursor* cursor = glfwCreateCursor(&cursor_image, 0, 0);
//	stbi_image_free(cursor_image.pixels);
//	glfwSetCursor(window, cursor);
//
//
//	////Create a new entity
//	//Entity player = ecs.newEntity();
//	//Transform& playerTransform = ecs.addComponent(player, Transform{ .x = 0, .y = 25, .xScale = 30, .yScale = 30 });
//	//ecs.addComponent(player, Sprite{});
//	//ecs.addComponent(player, Animator{});
//	//ecs.addComponent(player, Rigidbody{ .gravityScale = 0, .drag = 0, .friction = 0.2, .elasticity = 0 });
//	//ecs.addComponent(player, BoxCollider{ .isTrigger = true });
//	//BoxCollider& playerCollider = ecs.getComponent<BoxCollider>(player);
//	//Rigidbody& playerRigidbody = ecs.getComponent<Rigidbody>(player);
//	//TransformSystem::SetPosition(player, 0, 10, 0);
//
//	////Define the test animation
//	//Animator& animator = ecs.getComponent<Animator>(player);
//	//auto testAnims = AnimationsFromSpritesheet("warriorsheet.png", 8, 4, vector<int>(32, 100));
//	//AnimationSystem::AddAnimation(player, testAnims[0], "1");
//	//AnimationSystem::AddAnimation(player, testAnims[1], "2");
//	//AnimationSystem::PlayAnimation(player, "2", true);
//
//	//
//	//Create a new entity
//	Entity player = ecs.newEntity();
//	ecs.addComponent(player, Transform{ .x = 0, .y = 0, .xScale = 40, .yScale = 40, .yRotation = 0 });
//	ecs.addComponent(player, Sprite{});
//	ecs.addComponent(player, Animator{});
//	ecs.addComponent(player, Rigidbody{ .gravityScale = 0, .drag = 0,  .friction = 0, .elasticity = 0 });
//	ecs.addComponent(player, BoxCollider{});
//
//	//Define the test animation
//	Animator animator = ecs.getComponent<Animator>(player);
//	auto testAnims = AnimationsFromSpritesheet("assets/warriorattack.png", 4, 4, vector<int>(16, 200));
//	AnimationSystem::AddAnimation(player, testAnims[0], "1");
//	AnimationSystem::AddAnimation(player, testAnims[1], "2");
//	AnimationSystem::AddAnimation(player, testAnims[2], "3");
//	AnimationSystem::AddAnimation(player, testAnims[3], "4");
//	AnimationSystem::PlayAnimation(player, "2", true);
//
//
//	//Top-Right
//	Entity sprite2 = ecs.newEntity();
//	ecs.addComponent(sprite2, Transform{ .x = 300, .y = 200, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(sprite2, Sprite{ &texture });
//	ecs.addComponent(sprite2, Rigidbody{ .kinematic = true });
//	ecs.addComponent(sprite2, BoxCollider{ .scale = Vector2(10, 1) });
//	//Bottom-Left
//	Entity sprite3 = ecs.newEntity();
//	ecs.addComponent(sprite3, Transform{ .x = -300, .y = -200, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(sprite3, Sprite{ &texture });
//	ecs.addComponent(sprite3, Rigidbody{ .kinematic = true });
//	ecs.addComponent(sprite3, BoxCollider{});
//	//Top-Left
//	Entity sprite4 = ecs.newEntity();
//	ecs.addComponent(sprite4, Transform{ .x = -310, .y = 200, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(sprite4, Sprite{ &texture });
//	ecs.addComponent(sprite4, Rigidbody{ .drag = 0.1, .friction = 0.2, .elasticity = 0.125, .kinematic = false });
//	ecs.addComponent(sprite4, BoxCollider{});
//	//Bottom-Right
//	Entity sprite5 = ecs.newEntity();
//	ecs.addComponent(sprite5, Transform{ .x = 300, .y = -200, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(sprite5, Sprite{ &texture });
//	ecs.addComponent(sprite5, Rigidbody{ .velocity = Vector2(-985, 1000), .drag = 0.25, .elasticity = 0.125, .kinematic = false });
//	ecs.addComponent(sprite5, BoxCollider{});
//
//	
//	// create entity crosshair for gamepad
//	Entity crosshair = ecs.newEntity();
//	// adds crosshair texture
//	Transform crosshairTransform = ecs.addComponent(crosshair, Transform{ .x = 500, .y = 500, .xScale = 20, .yScale = 20 });
//	ecs.addComponent(crosshair, Sprite{ &texture2 });
//
//	vector <Entity>bullets;
//
//
//	float fireCooldown = 0.4f;
//	bool canFire = true;
//	
//
//
//	RenderSystem::SetBackgroundColor(100, 50, 1);
//
//	BoxCollider& collider = ecs.getComponent<BoxCollider>(player);
//	myMusic.Play();
//
//	//play sound files
//	mySpeaker4.Play(sound1);
//	//sets sound to loop, value 1=true
//	mySpeaker4.SetLooping(1);
//
//	//Game Loop
//	while (!glfwWindowShouldClose(window))
//	{
//
//		myMusic.updateBufferStream();
//
//
//		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
//		{
//			myMusic.Pause();
//			mySpeaker4.Pause();
//		}
//		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
//		{
//			myMusic.Resume();
//			mySpeaker4.Resume();
//		}
//		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
//		{
//			volume -= 0.01f;
//			if (volume < 0.0f) volume = 0.0f; // Clamp the volume to a minimum of 0.0f
//			myMusic.SetVolume(volume);
//		}
//		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
//		{
//			volume += 0.01f;
//			if (volume > 1.0f) volume = 1.0f; // Clamp the volume to a max 1.0f
//			myMusic.SetVolume(volume);
//		}
//
//		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//			glfwSetWindowShouldClose(window, true);
//
//		//test movement
//		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
//		{
//			engine.physicsSystem->Move(player, Vector2(500, 0) * engine.deltaTime);
//		}
//		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
//		{
//			engine.physicsSystem->Move(player, Vector2(-500, 0) * engine.deltaTime);
//		}
//		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
//		{
//			engine.physicsSystem->Move(player, Vector2(0, 500) * engine.deltaTime);
//		}
//		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
//		{
//			engine.physicsSystem->Move(player, Vector2(0, -500) * engine.deltaTime);
//		}
//		
//
//		Transform playerTransform = ecs.getComponent<Transform>(player);
//		engine.physicsSystem->Move(sprite3, Vector2(playerTransform.x, playerTransform.y) * engine.deltaTime);
//		engine.soundDevice->SetLocation(playerTransform.x, playerTransform.y, playerTransform.z);
//		engine.soundDevice->SetOrientation(0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
//
//		if (collider.collisions.size() > 0)
//		{
//			for (Collision c : collider.collisions)
//			{
//				cout << c.a << " " << c.b << endl;
//			}
//		}
//
//		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
//		{
//			Transform sprite2Transform = ecs.getComponent<Transform>(sprite2);
//			mySpeaker1.Play(sound3);
//			//engine.soundDevice->SetSourceLocation(1, sprite2Transform.x, sprite2Transform.y, 20.f);
//			mySpeaker1.SetLooping(0);
//			Transform sprite3Transform = ecs.getComponent<Transform>(sprite3);
//			//engine.soundDevice->SetSourceLocation(3, sprite3Transform.x, sprite3Transform.y, 2.f);
//			mySpeaker3.Play(sound4);
//		}
//
//		
//
//
//		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
//		{
//			Transform sprite4Transform = ecs.getComponent<Transform>(sprite4);
//			mySpeaker2.Play(sound2);
//			//engine.soundDevice->SetSourceLocation(2, sprite4Transform.x, sprite4Transform.y, 20.f);
//Transform sprite5Transform = ecs.getComponent<Transform>(sprite5);
//		//engine.soundDevice->SetSourceLocation(4, sprite5Transform.x, sprite5Transform.y, 0.f);
//		mySpeaker4.Play(sound2);
//		}
//
//
//		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
//		{
//			Transform sprite4Transform = ecs.getComponent<Transform>(sprite4);
//			mySpeaker2.Play(sound1);
//			//engine.soundDevice->SetSourceLocation(2, sprite4Transform.x, sprite4Transform.y, 20.f);
//		}
//		cam.SetPosition(playerTransform.x, playerTransform.y, playerTransform.z);
//
//		///////////////////////////////////////////////////////////////////////////////////////////////////////
//		/////////////////OHJAINSÄÄDÖT////////////////////////////////////////////////////
//		int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
//		if (present == GLFW_TRUE) {
//
//			GLFWgamepadstate state;
//
//			// set player speed
//			float playerSpeed = 500.0f;
//			//define deadzone
//			float deadzoneSize = 0.5f;
//			//calculate deadzone barrier
//			float deadzoneTreshold = deadzoneSize / 2.0f;
//			//Camera max distance
//			float max_distance = 30.0f;
//
//
//			int axisCount;
//			int buttonCount;
//			float radian = 5.0f;
//
//			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
//			const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
//
//
//			if (abs(axes[0]) > deadzoneTreshold || abs(axes[1]) > deadzoneTreshold)
//			{
//				// Sets movement vector based on movement speed
//				Vector2 movement(axes[0] * playerSpeed, -axes[1] * playerSpeed);
//
//				// move player based on movement vector
//				engine.physicsSystem->Move(player, movement * engine.deltaTime);
//
//			}
//
//			// sets crosshair position to zero
//			Vector2 crosshairPosition(0, 0);
//			if (abs(axes[2]) > deadzoneTreshold || abs(axes[3]) > deadzoneTreshold)
//			{
//				// Changes joystick cordinates into origo and scales it between -1, 1
//				Vector2 rightThumbstick(axes[2], axes[3]);
//				rightThumbstick = (rightThumbstick + Vector2(1.0f, 1.0f)) / 2.0f;
//				rightThumbstick -= Vector2(0.5f, 0.5f);
//				rightThumbstick *= 2.0f;
//
//				// creates vector toward joystick direction
//				float angle = atan2f(rightThumbstick.y, rightThumbstick.x);
//				Vector2 aimdirection = Vector2(cosf(angle), sinf(angle));
//
//				// normalizes vector and sets crosshair distance
//				aimdirection.Normalize();
//				aimdirection *= 150.0f;
//				ecs.getComponent<Sprite>(crosshair).enabled = true;
//				// crosshair position based on player position
//				Vector3 playerPosition(playerTransform.x, playerTransform.y, playerTransform.z);
//				Vector3 crosshairPosition = playerPosition + Vector3(aimdirection.x, -aimdirection.y, 0);
//				TransformSystem::SetPosition(crosshair, crosshairPosition);
//
//				if (fireCooldown <= 0)
//				{
//					if (axes[5] > 0.5f)
//					{
//
//						Entity bullet = ecs.newEntity();
//						ecs.addComponent(bullet, Transform{ .x = playerTransform.x + (aimdirection.x / 4), .y = playerTransform.y - (aimdirection.y / 4), .xScale = 5, .yScale = 5 });
//						ecs.addComponent(bullet, Sprite{ &texture3 });
//						ecs.addComponent(bullet, Rigidbody{ .velocity = Vector2(aimdirection.x * 40, -aimdirection.y * 40), .drag = 0, .elasticity = 0, .kinematic = true });
//						ecs.addComponent(bullet, BoxCollider{ .isTrigger = true });
//						bullets.push_back(bullet);
//						fireCooldown = 0.4f;
//					}
//				}
//				else
//				{
//					fireCooldown -= engine.deltaTime;
//				}
//
//				// set camera location between player and crosshair
//				//cam.SetPosition(playerTransform.x + (aimdirection.x / 8), playerTransform.y - (aimdirection.y / 8), playerTransform.z);
//			}
//			else
//			{
//				ecs.getComponent<Sprite>(crosshair).enabled = false;
//			}
//
//			for (const Entity& bullet : bullets)
//			{
//				auto hit = ecs.getComponent<BoxCollider>(bullet);
//				for (const Collision& collision : hit.collisions)
//				{
//					if (collision.type == Collision::Type::entityTrigger && collision.b != bullet)
//					{
//						ecs.destroyEntity(collision.b);
//						bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
//						ecs.destroyEntity(bullet);
//					}
//				}
//			}
//
//			if (buttons[0] == GLFW_PRESS)
//			{
//				Vector2 movement(0.0f, playerSpeed);
//				engine.physicsSystem->Move(player, movement * engine.deltaTime);
//			}
//			if (buttons[1] == GLFW_PRESS)
//			{
//				Transform sprite4Transform = ecs.getComponent<Transform>(sprite4);
//				mySpeaker2.Play(sound2);
//				//engine.soundDevice->SetSourceLocation(2, sprite4Transform.x, sprite4Transform.y, 20.f);
//			}
//
//		}
//
//		
//		///////////////////////////////////////////////////////////////////////////////////////////////////////
//		///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//		//Update all engine systems
//		engine.Update(&cam);
//
//
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//
//	return 0;
//}
