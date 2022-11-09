#include "engine/GameObject.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;
using namespace engine;

//Physics Constants
const float g = .48;
const float drag = .82;

int frame = 0;

//Player movement Variables
int moveSpeed = 7;
float fallSpeed = 0;
sf::Vector2f deltaPos;
float jumpPower = 0;
sf::Vector2f playerVelocity(0.f, 0.f);

//Button trackers
bool lmbDown, rmbDown, plusDown, spaceDown = false;

int main()
{

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(10, 10)),
		sf::Vertex(sf::Vector2f(150, 150))
	};

	//Create main level tilemap [ROW][COLUMN]
	vector<vector<uint8_t>> tilemap = LoadTilemap("map1");

	//Load textures
	sf::Texture playerTexture = loadTexture("pallo.png");
	sf::Texture woodTexture = loadTexture("wood1.png");
	sf::Texture strawberryTexture = loadTexture("strawberry.png");
	sf::Texture winnerTexture = loadTexture("winner.png");

	//Add map textures to list
	vector<sf::Texture> textures;
	textures.push_back(woodTexture);
	textures.push_back(playerTexture);
	sf::Sprite tilemapDrawerSprite(woodTexture);

	//Create player
	sf::Sprite player(playerTexture);
	player.setOrigin(player.getGlobalBounds().height / 2, player.getGlobalBounds().width / 2);
	player.setPosition(250, mapHeight * scale - scale);

	//Victory Textures
	sf::Sprite strawberry(strawberryTexture);
	strawberry.setPosition(100, 230);
	sf::Sprite winner(winnerTexture);
	winner.setOrigin(winner.getGlobalBounds().height / 2, winner.getGlobalBounds().width / 2);

	//Text stuff
	sf::Font arial;
	arial.loadFromFile(assetPath + "\\fonts\\ARIAL.TTF");
	sf::Text debugText;
	debugText.setFont(arial);
	debugText.setCharacterSize(25);

	//Create the window
	sf::View cam(sf::FloatRect(0, 0, 1000, 1000));
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game");

	vector<sf::Sprite*> toRender;
	toRender.push_back(&player);
	toRender.push_back(&strawberry);

	//cam.setCenter(500, player.getPosition().y + 300);
	window.setFramerateLimit(60);

	// run the program as long as the window is open
	while (window.isOpen())
	{
		frame++;
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

		//Victory stuff
		{
			strawberry.move(0, sinf((float)frame / 30) * 0.2);
			
			if(player.getGlobalBounds().intersects(strawberry.getGlobalBounds()))
			{
				strawberry.setPosition(-500, 0);
				winner.setPosition(cam.getCenter());
				toRender.push_back(&winner);
			}
		}

		//Draw based on map
		for (size_t x = 0; x < tilemap.size(); x++)
		{
			for (size_t y = 0; y < tilemap[x].size(); y++)
			{
				uint8_t spriteType = tilemap[x][y];
				if (spriteType != 0)
				{
					tilemapDrawerSprite.setTexture(textures[spriteType - 1]);
					tilemapDrawerSprite.setPosition(y * scale, x * scale);
					window.draw(tilemapDrawerSprite);
				}
			}
		}

		//Tilemap design and manipulation
		{
			//Add with left click
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				if (mousePos.y / scale < tilemap.size() && mousePos.y / scale >= 0 && mousePos.x / scale < tilemap[0].size() && mousePos.x / scale >= 0)
					tilemap[mousePos.y / scale][mousePos.x / scale] = 1;
			}

			//Clear with right click
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				if (mousePos.y / scale < tilemap.size() && mousePos.y / scale >= 0 && mousePos.x / scale < tilemap[0].size() && mousePos.x / scale >= 0)
					tilemap[mousePos.y / scale][mousePos.x / scale] = 0;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
			{
				if (!plusDown)
				{
					plusDown = true;
					SaveTilemap(tilemap, "map1");
				}
			}
			else
			{
				plusDown = false;
			}
		}
		/*
		//Camera
		{
			if (cam.getCenter().y - player.getPosition().y > 200)
			{
				cam.move(0, -5 * ((abs(cam.getCenter().y - player.getPosition().y) - 200) / 30));
			}
			if (cam.getCenter().y - player.getPosition().y < -200)
			{
				cam.move(0, 5 * ((abs(cam.getCenter().y - player.getPosition().y) - 200) / 30));
			}
			cam.setCenter(cam.getCenter().x, clamp(cam.getCenter().y, 500.f, float(mapHeight * scale - 500)));
		}*/
		
		//Player movement
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && playerVelocity.x == 0)
			{
				if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(-1 * moveSpeed, 0))) 
				{
					deltaPos.x += -1 * moveSpeed;
				}
				else 
				{
					for (int i = 2; i < 4; i++)
					{
						if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(deltaPos.x + (-1 * moveSpeed) / i, 0))) 
						{
							deltaPos.x += (-1 * moveSpeed) / i;
						}
					}

					if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, max(fallSpeed, 0.1f))) && jumpPower < 7)
					{
						jumpPower = 0;
						fallSpeed = 3;

						if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && !spaceDown) 
						{
							spaceDown = true;
							playerVelocity.x += 7;
							playerVelocity.y += -3;
							jumpPower = 15;
						}
					}
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && playerVelocity.x == 0)
			{
				if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(1 * moveSpeed, 0)))
				{
					deltaPos.x += 1 * moveSpeed;
				}
				else
				{
					for (int i = 2; i < 4; i++)
					{
						if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(deltaPos.x + (1 * moveSpeed) / i, 0)))
						{
							deltaPos.x += (1 * moveSpeed) / i;
						}
					}

					if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, max(fallSpeed, 0.1f))) && jumpPower < 7)
					{
						jumpPower = 0;
						fallSpeed = 3;

						if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && !spaceDown)
						{
							spaceDown = true;
							playerVelocity.x += -7;
							playerVelocity.y += -3;
							jumpPower = 15;
						}
					}
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				spaceDown = true;
				if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, -1 * jumpPower)))
				{
					deltaPos.y += -1 * jumpPower;
					if (jumpPower > 0)
						jumpPower -= g;
					else
						jumpPower = 0;
				}
				else
				{
					for (int i = 2; i < 4; i++)
					{
						if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, deltaPos.y + (-1 * jumpPower) / i)))
						{
							deltaPos.y += (-1 * jumpPower) / i;
						}
						else
						{
							fallSpeed = 0;
							jumpPower = 0;
						}
					}
				}
			}
			else 
			{
				spaceDown = false;
				jumpPower = 0;
			}
		}

		//Gravity Physics
		if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, max(fallSpeed, 0.01f))))
		{
			deltaPos.y += fallSpeed;
			if(fallSpeed < 20)
				fallSpeed += g;
		}
		else
		{
			for (int i = 2; i < 16; i++)
			{
				if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, 2 / i)))
				{
					deltaPos.y += 2 / i;
				}
				else
				{
					fallSpeed = 0;
					jumpPower = 15;
				}
			}
		}

		//Apply player velocity
		deltaPos += playerVelocity;

		if(playerVelocity.x > 0)
			playerVelocity.x -= drag;
		if(playerVelocity.x < 0)
			playerVelocity.x += drag;
		if (abs(playerVelocity.x) <= drag)
			playerVelocity.x = 0;
		
		if(playerVelocity.y > 0)
			playerVelocity.y -= drag;
		if(playerVelocity.y < 0)
			playerVelocity.y += drag;
		if (abs(playerVelocity.y) <= drag)
			playerVelocity.y = 0;

		if (CheckTilemapCollision(player, tilemap, sf::Vector2f(deltaPos.x, 0))) 
		{
			deltaPos.x = 0;
		}
		if (CheckTilemapCollision(player, tilemap, sf::Vector2f(0, deltaPos.y))) 
		{
			deltaPos.y = 0;
		}
		if (CheckTilemapCollision(player, tilemap, deltaPos))
		{
			deltaPos.y = 0;
		}

		player.move(deltaPos);
		deltaPos = sf::Vector2f();

		// draw everything here
		window.setView(cam);
		for (int i = 0; i < toRender.size(); i++)
		{
			window.draw(*toRender[i]);
		}

		window.draw(line, 2, sf::Lines);

		// end the current frame
		window.display();
	}

	return 0;
}