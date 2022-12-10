#include "engine/Application.h"

//Create instances of the ECS controller and the standard engine library
ECS ecs;

using namespace engine;

int main()
{
	EngineLib lib;

	lib.physicsSystem->Init(0.f, 9.81f);

	//Create player
	Entity player = ecs.newEntity();
	ecs.addComponent(player, Transform{ .x = 100, .y = 100, .xScale = 1, .yScale = 1 });
	ecs.addComponent(player, Sprite());
	ecs.addComponent(player, Animator());
	ecs.addComponent(player, Rigidbody());

	//Load the player's spritesheet
	sf::Image spritesheet = LoadImage("Knight Sprites.png");
	sf::Texture playerTexture = CustomSlice(spritesheet, 0, 0, 16, 16);
	vector<sf::Texture> textures = SliceSpritesheet(spritesheet, 16, 16);

	//Set the players default texture
	ecs.getComponent<Sprite>(player).texture = playerTexture;

	//Add animations to player automatically sliced from the spritesheet
	lib.animationSystem->AddAnimations(player, AnimationsFromSpritesheet(spritesheet, 16, 16, vector<int>(8, 250)), vector<string>{"Down", "Left", "Up", "Right"});

	//Define player's physics body
	lib.physicsSystem->DefineBody(player, 16.f, 16.f);


	//Create gound
	Entity ground = ecs.newEntity();
	ecs.addComponent(ground, Transform{ .x = 321, .y = 300, .xScale = 10, .yScale = .25 });
	ecs.addComponent(ground, Sprite());
	ecs.addComponent(ground, Rigidbody());

	sf::Texture woodTexture = LoadTexture("wood2.png");
	ecs.getComponent<Sprite>(ground).texture = woodTexture;

	//Define ground's physics body
	lib.physicsSystem->DefineBody(ground, 640, 16, true);


	//Play the "Down" animation of player on repeat
	//lib.animationSystem->PlayAnimation(player, "Down", true);

	tmx::Map map;
	map.load("assets/untitled.tmx");

	//SFML window
	sf::RenderWindow window(sf::VideoMode(800, 600), "test");

	//Main game loop
	while (window.isOpen())
	{
		//SFML window close event
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
		window.clear(sf::Color::Black);

		//Run the animationSystem's Update method each frame
		lib.animationSystem->Update();

		RenderTilemap(&map, &window);

		//Run the renderSystem's Render method each frame
		lib.renderSystem->Update(window);
		lib.animationSystem->Update();
		lib.physicsSystem->Update();

		//SFML display window
		window.display();
	}
}