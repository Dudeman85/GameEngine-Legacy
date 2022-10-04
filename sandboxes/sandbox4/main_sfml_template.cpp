#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <vector>

#if defined(_DEBUG)
std::string ASSET_PATH = "assets/";
#else
std::string ASSET_PATH = "assets/";
#endif

using namespace std;

//Constants
const float g = .22;

//Player movement Variables
int scale = 50;
int moveSpeed = 10;
float fallSpeed = 0;
sf::Vector2f deltaPos;
float jumpPower = 0;

//Mouse button trackers
bool lmbDown = false;
bool rmbDown = false;


class Platform
{
public:
	Platform(sf::Vector2f pos1, sf::Vector2f pos2, sf::Texture platformTexture)
	{
		topLeft = sf::Vector2f(min(pos1.x, pos2.x), min(pos1.y, pos2.y));
		bottomRight = sf::Vector2f(max(pos1.x, pos2.x), max(pos1.y, pos2.y));
		texture = platformTexture;
	}

	sf::Texture texture;
	sf::Vector2f topLeft;
	sf::Vector2f bottomRight;

	sf::RectangleShape draw()
	{
		sf::RectangleShape rectangle;
		rectangle.setPosition(topLeft);
		rectangle.setSize(bottomRight - topLeft);
		rectangle.setTexture(&texture);

		return rectangle;
	}
	sf::FloatRect getGlobalBounds()
	{
		return sf::FloatRect(topLeft, sf::Vector2f(bottomRight - topLeft));
	}
};

//Load texture with error checking
sf::Texture loadTexture(string name)
{
	sf::Texture texture;
	if (!texture.loadFromFile(ASSET_PATH + name))
	{
		printf("Load texture error");
	}
	return texture;
}

//Returns true if any corner of sprite is overlapping a non zero position of the tilemap.
//Assumes origin of sprite is in the center. Optional offset for shifting the hitbox.
bool CheckTilemapCollision(sf::Sprite sprite, vector<vector<uint8_t>> tilemap, sf::Vector2f offset = sf::Vector2f(0, 0))
{
	sf::Vector2f position = sprite.getPosition() + offset;
	int width = sprite.getGlobalBounds().width / 2;
	int height = sprite.getGlobalBounds().height / 2;

	return (tilemap[(position.y - height) / scale][(position.x - width) / scale] != 0) ||
		(tilemap[(position.y - height) / scale][(position.x + width) / scale] != 0) ||
		(tilemap[(position.y + height) / scale][(position.x - width) / scale] != 0) ||
		(tilemap[(position.y + height) / scale][(position.x + width) / scale] != 0);
}

int main()
{
	//Create main level tilemap [ROW][COLUMN]
	vector<vector<uint8_t>> tilemap(21, vector<uint8_t>(20, 1));

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
	player.setPosition(250, 250);

	sf::Sprite tilemapDrawerSprite(woodTexture);

	//Old variables for drawing platforms
	sf::Vector2i pos1;
	sf::Vector2i pos2;
	sf::Vector2i mousePos;
	sf::RectangleShape tempPlatform(sf::Vector2f(0, 0));
	tempPlatform.setTexture(&texture);
	vector<Platform> platforms;

	//Create the window
	sf::RenderWindow window(sf::VideoMode(20 * scale, 21 * scale), "Up game");
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

		/*
		//Platform mouse drawing and deleting
		{
			//Draw all saved boxes
			for (int i = 0; i < platforms.size(); i++)
			{
				window.draw(platforms[i].draw());
			}

			//Realtime Draw new box
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!lmbDown)
				{
					pos1 = sf::Mouse::getPosition(window);
					lmbDown = true;
				}
				if (lmbDown)
				{
					pos2 = sf::Mouse::getPosition(window);

					//Draw the box
					tempPlatform.setSize(sf::Vector2f(abs(pos1.x - pos2.x), abs(pos1.y - pos2.y)));
					tempPlatform.setPosition(sf::Vector2f(std::min(pos1.x, pos2.x), std::min(pos1.y, pos2.y)));
					window.draw(tempPlatform);
				}
			}
			else
			{
				if (lmbDown)
				{
					lmbDown = false;
					platforms.push_back(Platform((sf::Vector2f)pos1, (sf::Vector2f)pos2, texture));

					//draw to prevent 1 frame flicker
					window.draw(platforms[platforms.size() - 1].draw());
				}
			}

			//delete with right click
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !rmbDown)
			{
				rmbDown = true;
				for (int i = platforms.size() - 1; i >= 0; i--)
				{
					mousePos = sf::Mouse::getPosition(window);
					if (mousePos.x > platforms[i].topLeft.x && mousePos.x < platforms[i].bottomRight.x && mousePos.y > platforms[i].topLeft.y && mousePos.y < platforms[i].bottomRight.y)
					{
						platforms.erase(platforms.begin() + i);
						break;
					}
				}
			}
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				rmbDown = false;
			}
		}*/

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
							deltaPos.y += (-1 * jumpPower) / (i + 1);
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
					jumpPower = 10;
				}
			}
		}

		player.move(deltaPos);
		deltaPos = sf::Vector2f();

		// draw everything here
		window.draw(player);

		// end the current frame
		window.display();
	}

	return 0;
}