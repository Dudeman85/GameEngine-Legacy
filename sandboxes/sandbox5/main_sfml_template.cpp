#include <iostream>
#include "engine/ECSCore.h"

struct Position
{
	int x;
	int y;
};

int main()
{
	ECS ecs;

	Entity player = ecs.newEntity();
	ecs.addComponent(player, Position{.x = 4, .y = 10});

	/*
	//Create the window
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game");

	//cam.setCenter(500, player.getPosition().y + 300);
	window.setFramerateLimit(60);

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}


		// clear the window with black color
		window.clear(sf::Color::Black);

		window.draw(line, 2, sf::Lines);

		// end the current frame
		window.display();
	}
	*/
	return 0;
}