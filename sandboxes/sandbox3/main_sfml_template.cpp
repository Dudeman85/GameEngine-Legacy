#include "engine/Application.h"
ECS ecs;


int main()
{
	engine::EngineLib lib;
	sf::RenderWindow window(sf::VideoMode(800, 600), "test");
	Entity player = ecs.newEntity();
	ecs.addComponent(player, engine::Transform{ .x = 100, .y = 100, .xScale = 1, .yScale = 1 });
	ecs.addComponent(player, engine::Sprite());
	ecs.addComponent(player, engine::Rigidbody());
	sf::Texture defaultTexture = engine::LoadTexture("Ghost.png");
	ecs.getComponent<engine::Sprite>(player).texture = defaultTexture;

	lib.physicsSystem->Init(0.1f, 0.5f);
	lib.physicsSystem->DefineBody(player, 64.f, 64.f);

	while (window.isOpen())
	{
		//SFML window close event
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
			
		window.clear(sf::Color::Black);
		
		
		lib.physicsSystem->Update();
		lib.transformSystem->Update();
		lib.renderSystem->Update(window);

		//SFML display window
		window.display();
	}
}