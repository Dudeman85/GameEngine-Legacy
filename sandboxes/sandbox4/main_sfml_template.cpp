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

sf::Texture loadTexture(string name)
{
	sf::Texture texture;
	if (!texture.loadFromFile(ASSET_PATH + name))
	{
		printf("Load texture error");
	}
	return texture;
}

class GameObject
{
public:
	GameObject() {};
	GameObject(int x, int y)
	{
		position.x = x;
		position.y = y;
	}

	void move(float dx, float dy)
	{
		position.x += dx;
		position.y += dy;
	}

	sf::Vector2f position;
};

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

bool CheckCollision(Platform platform, sf::Sprite player)
{
	return player.getGlobalBounds().intersects(platform.getGlobalBounds());
}

int main()
{
	int scale = 64;
	int moveSpeed = 10;
	float fallSpeed = 0;
	float g = .42;
	bool lmbDown = false;
	bool rmbDown = false;

	sf::Texture texture = loadTexture("image1.png");
	sf::Texture woodTexture = loadTexture("image.png");
	sf::Texture wood2Texture = loadTexture("wood2.png");
	sf::Texture playerTexture = loadTexture("pallo.png");

	sf::Sprite player(playerTexture);
	player.setPosition(250, 250);

	sf::Sprite mySprite(woodTexture);

	sf::RectangleShape tempPlatform(sf::Vector2f(0, 0));
	tempPlatform.setTexture(&texture);
	sf::Vector2i pos1;
	sf::Vector2i pos2;
	vector<Platform> platforms;
	sf::Vector2i mousePos;

	// create the window
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
	window.setFramerateLimit(60);

	vector<sf::Texture> textures;
	textures.push_back(woodTexture);
	textures.push_back(wood2Texture);

	vector<vector<uint8_t>> map = {
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
		{0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	vector<GameObject> players = {
		GameObject(0, 0)
	};

	vector<GameObject> enemies = {
		GameObject(0, 0)
	};

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
		for (size_t x = 0; x < map.size(); x++)
		{
			for (size_t y = 0; y < map[x].size(); y++)
			{
				auto spriteType = map[x][y];
				if (spriteType != 0)
				{
					mySprite.setTexture(textures[spriteType - 1]);
					mySprite.setPosition(y * scale, x * scale);
					window.draw(mySprite);
				}
			}
		}

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
		}

		//Player movement
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				player.move(-1 * moveSpeed, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				player.move(1 * moveSpeed, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				player.move(0, -1 * moveSpeed);
			}
		}

		for (int i = 0; i < platforms.size(); i++)
		{
			cout << CheckCollision(platforms[i], player) << endl;
		}

		if (player.getPosition().y <= 500)
		{
			player.move(0, fallSpeed);
			fallSpeed += g;
		}
		else
		{
			fallSpeed = 0;
		}

		// draw everything here...
		window.draw(player);

		// end the current frame
		window.display();
	}

	return 0;
}