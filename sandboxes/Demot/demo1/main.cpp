#include "engine/Application.h"
#include "Camera.cpp"
#include "Sound.h"
#include "PlayerController.h"
//Create instances of the ECS controller and the standard engine library
ECS ecs;

using namespace engine;

int main()
{
	EngineLib lib;
	ecs.registerComponent<Player>();
	shared_ptr <PlayerControl>playerControlSystem = ecs.registerSystem<PlayerControl>();
	Signature playerControlSystemSignature;
	playerControlSystemSignature.set(ecs.getComponentId<Transform>());
	playerControlSystemSignature.set(ecs.getComponentId<Rigidbody>());
	playerControlSystemSignature.set(ecs.getComponentId<Player>());
	playerControlSystemSignature.set(ecs.getComponentId<Animator>());
	ecs.setSystemSignature<PlayerControl>(playerControlSystemSignature);
	playerControlSystem->lib = &lib;
	shared_ptr<CameraSystem> cameraSystem;
	ecs.registerComponent<Camera>();

	//Animation System
	cameraSystem = ecs.registerSystem<CameraSystem>();
	Signature cameraSystemSignature;
	cameraSystemSignature.set(ecs.getComponentId<Camera>());
	cameraSystemSignature.set(ecs.getComponentId<Transform>());
	ecs.setSystemSignature<CameraSystem>(cameraSystemSignature);

	lib.physicsSystem->Init(-3.f, 9.81f);

	//Create player
	Entity player = ecs.newEntity();
	ecs.addComponent(player, Transform{ .x = 100, .y = 100, .xScale = 1, .yScale = 1 });
	ecs.addComponent(player, Sprite());
	ecs.addComponent(player, Animator());
	ecs.addComponent(player, Rigidbody());
	ecs.addComponent(player, Player());

	//Load the player's spritesheet
	sf::Image spritesheet = LoadImage("Knight Sprites.png");
	sf::Texture playerTexture = CustomSlice(spritesheet, 0, 0, 16, 16);
	vector<sf::Texture> textures = SliceSpritesheet(spritesheet, 16, 16);

	//Set the players default texture
	ecs.getComponent<Sprite>(player).texture = playerTexture;

	//Add animations to player automatically sliced from the spritesheet
	lib.animationSystem->AddAnimations(player, AnimationsFromSpritesheet(spritesheet, 16, 16, vector<int>(8, 125)), vector<string>{"Down", "Left", "Up", "Right"});

	//Define player's physics body
	lib.physicsSystem->DefineBody(player, 16.f, 16.f);


	//Create Camera
	Entity camera = ecs.newEntity();
	ecs.addComponent(camera, Transform{.x = 100, .y = 100});
	ecs.addComponent(camera, Camera{ .cam = sf::View(sf::FloatRect(0, 0, 1000, 1000)), .followDistance = 100, .target = player });
	ecs.getComponent<Camera>(camera).cam.zoom(.45f);
	
	// Creating wallLeft
	Entity wallLeft = ecs.newEntity();
	ecs.addComponent(wallLeft, Transform{ .x = 7.5, .y = 160 });
	ecs.addComponent(wallLeft, Rigidbody());

	// Defining wallLeft's physics body
	lib.physicsSystem->DefineBody(wallLeft, 16, 320, true);

	// Creating floor
	Entity floor = ecs.newEntity();
	ecs.addComponent(floor, Transform{.x = 400, .y = 312});
	ecs.addComponent(floor, Rigidbody());

	// Defining floor's physics body
	lib.physicsSystem->DefineBody(floor, 800, 16, true);

	// Creating platform1
	Entity platform1 = ecs.newEntity();
	ecs.addComponent(platform1, Transform{ .x = 137, .y = 280 });
	ecs.addComponent(platform1, Rigidbody());

	// Defining platform1's physics body
	lib.physicsSystem->DefineBody(platform1, 78, 16, true);

	// Creating platform2
	Entity platform2 = ecs.newEntity();
	ecs.addComponent(platform2, Transform{ .x = 249, .y = 248 });
	ecs.addComponent(platform2, Rigidbody());

	// Defining platform2's physics body
	lib.physicsSystem->DefineBody(platform2, 78, 16, true, 1, 0.1);

	// Creating platform3
	Entity platform3 = ecs.newEntity();
	ecs.addComponent(platform3, Transform{ .x = 377, .y = 216 });
	ecs.addComponent(platform3, Rigidbody());

	// Defining platform3's physics body
	lib.physicsSystem->DefineBody(platform3, 78, 16, true);

	// Creating platform4
	Entity platform4 = ecs.newEntity();
	ecs.addComponent(platform4, Transform{ .x = 489, .y = 184 });
	ecs.addComponent(platform4, Rigidbody());

	// Defining platform4's physics body
	lib.physicsSystem->DefineBody(platform4, 78, 16, true);

	// Creating platform5
	Entity platform5 = ecs.newEntity();
	ecs.addComponent(platform5, Transform{ .x = 617, .y = 168 });
	ecs.addComponent(platform5, Rigidbody());

	// Defining platform5's physics body
	lib.physicsSystem->DefineBody(platform5, 78, 16, true);

	//Play the "Down" animation of player on repeat
	//lib.animationSystem->PlayAnimation(player, "Down", true);

	tmx::Map map;
	map.load("assets/Demo1Tilemap.tmx");

	//SFML window
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "test");

	// Audio
	playSoundfile("spring-weather-1.wav");

	//Main game loop
	while (window.isOpen())
	{
		//SFML window close event
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
		window.clear(sf::Color::Black);

		RenderTilemap(&map, &window);

		//Run the renderSystem's Render method each frame
		lib.renderSystem->Update(window);
		lib.animationSystem->Update();
		lib.physicsSystem->Update();
		playerControlSystem->Update();

		cameraSystem->Update();

		window.setView(ecs.getComponent<Camera>(camera).cam);

		//SFML display window
		window.display();
	}
}