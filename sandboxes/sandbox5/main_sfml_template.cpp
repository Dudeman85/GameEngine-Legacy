#include "engine/GameObject.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <vector>

class Vector
{
public:
	sf::Vector2f start;
	float angle;
	float length;
};

int main()
{

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(250, 250)),
		sf::Vertex(sf::Vector2f(150, 150))
	};

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

		line[1].position = sf::Vector2f(sf::Mouse::getPosition(window));




		// clear the window with black color
		window.clear(sf::Color::Black);

		window.draw(line, 2, sf::Lines);

		// end the current frame
		window.display();
	}

	return 0;
}