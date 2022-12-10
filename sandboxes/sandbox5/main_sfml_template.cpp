#include "engine/Application.h"

ECS ecs;

using namespace engine;

int main()
{
	EngineLib lib;

	Entity player = ecs.newEntity();
	ecs.addComponent(player, Transform{ .x = 100, .y = 100 });
	

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


		lib.transformSystem->Update();
		lib.renderSystem->Update(window);

		//SFML display window
		window.display();
	}
}