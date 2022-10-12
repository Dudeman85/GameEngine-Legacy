#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <vector>
#include <libs.h>

using namespace std;

//Physics Constants
const float g = .42;
const float drag = .82;

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
	//Create main level tilemap [ROW][COLUMN]
	vector<vector<uint8_t>> tilemap = LoadTilemap("map1");

	//Load textures
	sf::Texture texture = loadTexture("image1.png");
	sf::Texture playerTexture = loadTexture("pallo.png");
	sf::Texture woodTexture = loadTexture("wood1.png");
	sf::Texture wood2Texture = loadTexture("wood2.png");

	//Add map textures to list
	vector<sf::Texture> textures;
	textures.push_back(woodTexture);
	textures.push_back(wood2Texture);

	//Create player
	sf::Sprite player(playerTexture);
	player.setOrigin(player.getGlobalBounds().height / 2, player.getGlobalBounds().width / 2);
	player.setPosition(250, mapHeight * scale + scale);

	sf::Sprite tilemapDrawerSprite(woodTexture);

	//Debug and dev stuff
	sf::Font arial;
	arial.loadFromFile(assetPath + "\\fonts\\ARIAL.TTF");
	sf::Text debugText;
	debugText.setFont(arial);
	debugText.setCharacterSize(25);

	//Create the window
	sf::View cam(sf::FloatRect(0, 0, 1000, 1000));
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

		//Draw based on map
		for (size_t x = 0; x < tilemap.size(); x++)
		{
			for (size_t y = 0; y < tilemap[x].size(); y++)
			{
				auto spriteType = tilemap[x][y];
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
				if (window.mapPixelToCoords(sf::Mouse::getPosition(window)).y / scale < tilemap.size() && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x / scale < tilemap[0].size())
					tilemap[window.mapPixelToCoords(sf::Mouse::getPosition(window)).y / scale][window.mapPixelToCoords(sf::Mouse::getPosition(window)).x / scale] = 1;
			}

			//Clear with right click
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				if (window.mapPixelToCoords(sf::Mouse::getPosition(window)).y / scale < tilemap.size() && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x / scale < tilemap[0].size())
					tilemap[window.mapPixelToCoords(sf::Mouse::getPosition(window)).y / scale][window.mapPixelToCoords(sf::Mouse::getPosition(window)).x / scale] = 0;
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

		//Camera
		{
			if (cam.getCenter().y - player.getPosition().y > 300)
			{
				cam.move(0, -5 * ((abs(cam.getCenter().y - player.getPosition().y) - 300) / 30));
			}
			if (cam.getCenter().y - player.getPosition().y < -300)
			{
				cam.move(0, 5 * ((abs(cam.getCenter().y - player.getPosition().y) - 300) / 30));
			}
		}

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

					if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, max(fallSpeed, 0.01f))))
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

					if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, max(fallSpeed, 0.01f))))
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

		player.move(deltaPos);
		deltaPos = sf::Vector2f();

		// draw everything here
		window.setView(cam);
		window.draw(player);
		window.draw(debugText);

		// end the current frame
		window.display();
	}

	return 0;
}