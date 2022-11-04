#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <vector>



#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

#include <libs.h>


//#include <AudioDemo/AudioDemo.h>



using namespace std;

//Constants
const float g = .42;

//Player movement Variables
int moveSpeed = 10;
float fallSpeed = 0;
sf::Vector2f deltaPos;
float jumpPower = 0;

//Button trackers
bool lmbDown, rmbDown, plusDown = false;

int main()
{
	//Create main level tilemap [ROW][COLUMN]
	vector<vector<uint8_t>> tilemap = LoadTilemap("map1");

	//Load textures
	sf::Texture texture = loadTexture("image.png");
	sf::Texture playerTexture = loadTexture("player.png");
	sf::Texture woodTexture = loadTexture("wall.png");
	sf::Texture wood2Texture = loadTexture("wall2.png");

	//Add map textures to list
	vector<sf::Texture> textures;
	textures.push_back(woodTexture);
	textures.push_back(wood2Texture);

	//Create player
	sf::Sprite player(playerTexture);
	player.setOrigin(player.getGlobalBounds().height / 2, player.getGlobalBounds().width / 2);
	player.setPosition(250, 250);

	sf::Sprite tilemapDrawerSprite(woodTexture);

	//Debug and dev stuff
	sf::Font arial;
	arial.loadFromFile(assetPath + "\\fonts\\ARIAL.TTF");
	sf::Text debugText;
	debugText.setFont(arial);
	debugText.setCharacterSize(25);

	//Create the window
	sf::RenderWindow window(sf::VideoMode(mapWidth * scale, mapHeight * scale), "Game");
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
				if (sf::Mouse::getPosition(window).y / scale < tilemap.size() && sf::Mouse::getPosition(window).x / scale < tilemap[0].size())
					tilemap[sf::Mouse::getPosition(window).y / scale][sf::Mouse::getPosition(window).x / scale] = 1;
			}

			//Clear with right click
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				if (sf::Mouse::getPosition(window).y / scale < tilemap.size() && sf::Mouse::getPosition(window).x / scale < tilemap[0].size())
					tilemap[sf::Mouse::getPosition(window).y / scale][sf::Mouse::getPosition(window).x / scale] = 0;
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

		//Player movement
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(-1 * moveSpeed, 0)))
				{
					deltaPos.x += -1 * moveSpeed;
				}
				else
				{
					for (int i = 2; i < 4; i++)
					{
						if (!CheckTilemapCollision(player, tilemap, sf::Vector2f((-1 * moveSpeed) / i, 0)))
						{
							deltaPos.x += (-1 * moveSpeed) / i;
						}
					}
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(1 * moveSpeed, 0)))
				{
					deltaPos.x += 1 * moveSpeed;
				}
				else
				{
					for (int i = 2; i < 4; i++)
					{
						if (!CheckTilemapCollision(player, tilemap, sf::Vector2f((1 * moveSpeed) / i, 0)))
						{
							deltaPos.x += (1 * moveSpeed) / i;
						}
					}
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
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
						if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, (-1 * jumpPower) / i)))
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
				jumpPower = 0;
			}
		}

		//Gravity Physics
		if (!CheckTilemapCollision(player, tilemap, sf::Vector2f(0, max(fallSpeed, 0.01f))))
		{
			deltaPos.y += fallSpeed;
			if (fallSpeed < 20)
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

		player.move(deltaPos);
		deltaPos = sf::Vector2f();

		// draw everything here
		window.draw(player);
		window.draw(debugText);

		// end the current frame
		window.display();

		// sound effects
		//init:
		// Muokataan vielä:
		/*

			auto result = ma_engine_init(NULL, &audioEngine);
			if (result != MA_SUCCESS) {
				throw std::runtime_error("Failed to initialize audio engine!");
				return;
			}

			// deinit:

			ma_engine_uninit(&audioEngine);


			void Window::playSound(const std::string & fileName) {
				auto result = ma_engine_play_sound(&init.audioEngine, fileName.c_str(), NULL);
				if (result != MA_SUCCESS) {
					throw std::runtime_error("Failed to play sound!");
					return;
				}
			}

		}
		*/

	}
	return 0;
}
