#include "engine/Application.h"

//Create instances of the ECS controller and the standard engine library
ECS ecs;

int main()
{
	engine::EngineLib lib;
	//Create a new entity and add the Transform, Sprite, and Animator components
	Entity player = ecs.newEntity();
	ecs.addComponent(player, engine::Transform{ .x = 100, .y = 100, .xScale = 10, .yScale = 10 });
	ecs.addComponent(player, engine::Sprite());
	ecs.addComponent(player, engine::Animator());

	//Load the spritesheet
	sf::Image spritesheet = engine::LoadImage("Knight Sprites.png");
	sf::Texture defaultTexture = engine::CustomSlice(spritesheet, 0, 0, 16, 16);

	vector<sf::Texture> textures = engine::SliceSpritesheet(spritesheet, 16, 16);

	//Set the players default texture
	ecs.getComponent<engine::Sprite>(player).texture = defaultTexture;

	//Add animations to player automatically sliced from the spritesheet
	lib.animationSystem->AddAnimations(player, engine::AnimationsFromSpritesheet(spritesheet, 16, 16, vector<int>(8, 250)), vector<string>{"Down", "Left", "Up", "Right"});

	//Play the "Down" animation of player on repeat
	lib.animationSystem->PlayAnimation(player, "Down", true);

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

		engine::RenderTilemap(&map, &window);

		//Run the renderSystem's Render method each frame
		lib.renderSystem->Update(window);


		//SFML display window
		window.display();
	}
}