#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "Sprite.h"
#include <cmath>

using namespace std;

namespace engine
{
	class GameObject
	{
	private:
		map<string, engine::Animation> animations;
		sf::Sprite sprite;

		// Enemy 
		sf::RectangleShape enemy;

		

		sf::Clock animationTimer;
		int animationFrame = 0;
		bool repeatAnimation = false;

		void advanceFrame()
		{
			//Change GameObject texture
			texture = animations[currentAnimation].textures[animationFrame];

			animationTimer.restart();
			animationFrame++;

			//If end of animation has been reached go to start or end animation
			if (animationFrame >= animations[currentAnimation].length)
			{
				animationFrame = 0;
				if (!repeatAnimation)
				{
					playingAnimation = false;
					currentAnimation = "";
				}
			}
		}

	public:
		bool enabled = true;
		bool playingAnimation = false;
		string currentAnimation = "";
		int x = 0;
		int y = 0;
		float xScale = 1;
		float yScale = 1;
		float rotationAngle = 0;

		sf::Texture texture;

		GameObject() {}
		GameObject(sf::Texture& _texture) : texture(_texture) {}

		//Sets absolute position of GameObject
		void setPosition(int xPos, int yPos)
		{
			x = xPos;
			y = yPos;
			sprite.setPosition(x, y);
		}
		//Moves the GameObject a relative distance
		void move(int dx, int dy)
		{
			x += dx;
			y += dy;
			sprite.setPosition(x, y);
		}
		//Sets the scale multiplier
		void setScale(float scale)
		{
			xScale = scale;
			yScale = scale;
		}
		//Sets the scale multiplier
		void setScale(float x, float y)
		{
			xScale = x;
			yScale = y;
		}

		//Updates the GameObject. Call this every frame for every GameObject
		void update()
		{
			if (enabled)
			{
				sprite.setPosition(x, y);
				sprite.setScale(xScale, yScale);
				sprite.setRotation(rotationAngle);

				if (playingAnimation)
				{
					//If delay ms has passed advance animation frame
					if (animationTimer.getElapsedTime().asMilliseconds() >= animations[currentAnimation].delays[animationFrame])
					{
						advanceFrame();
					}
				}
			}
		}

		//Render GameObject to window
		sf::Sprite draw()
		{
			if(!playingAnimation)
				sprite.setTexture(texture, true);

			if (!enabled)
			{
				sprite.setScale(0, 0);
			}

			return sprite;
		}

		//Animation methods

		//Play an Animation
		//If repeat is set to true it will loop untill stopAnimation is called
		void playAnimation(string name, bool repeat = false)
		{
			playingAnimation = true;
			currentAnimation = name;
			animationFrame = 0;
			repeatAnimation = repeat;
			animationTimer.restart();
		}

		//Stop current animation
		void stopAnimation()
		{
			playingAnimation = false;
			currentAnimation = "";
			animationFrame = 0;
		}

		//Add an animation with custom delays in ms
		void addAnimation(vector<sf::Texture> textures, vector<int> delays, string name)
		{
			engine::Animation newAnimation;
			newAnimation.Load(textures, delays);

			animations[name] = newAnimation;
		}
		//Add an animation with equal delays in ms
		void addAnimation(vector<sf::Texture> textures, int delays, string name)
		{
			engine::Animation newAnimation;
			newAnimation.Load(textures, delays);

			animations[name] = newAnimation;
		}
		//Add an already made animation
		void addAnimation(engine::Animation newAnimation, string name)
		{
			animations[name] = newAnimation;
		}

		//Automatically slice and add animations from a spritemap with equal delays in ms and optional names.
		//Adds one animation per row of sprites in the spritemap and sets every frame delay to be the same. All sprites must have the same width and height.
		//Optionally give a vector of strings for animation names, otherwise they will be named by the number of their row in the tilemap.
		void autoAddAnimations(sf::Image spritemap, int width, int height, int delays, vector<string> names = vector<string>())
		{
			//Get a list of textures from the spritemap
			vector<sf::Texture> textures;
			textures = engine::SliceSpritemap(spritemap, width, height);

			//For each row in the spritemap
			for (size_t y = 0; y < floor(spritemap.getSize().y / height); y++)
			{
				//Frame buffer to add to animation
				vector<sf::Texture> animationSlice;
				//For each column in the spritemap
				for (size_t x = 0; x < floor(spritemap.getSize().x / width); x++)
				{
					//Add the next texture to the buffer 
					animationSlice.push_back(textures[x]);
				}

				//Create new animation and add buffer frames to it
				engine::Animation newAnimation;
				newAnimation.Load(animationSlice, delays);

				//If there are names in the names list add the animation with that
				if (names.size() > y)
					animations[names[y]] = newAnimation;
				else
					//If not name it the index of the row
					animations[to_string(y)] = newAnimation;
			}
		}

		//Automatically slice and add animations from a spritemap with custom delays in ms and optional names.
		//Adds one animation per row of sprites in the spritemap and sets every frame delay to be the same. All sprites must have the same width and height.
		//Optionally give a vector of strings for animation names, otherwise they will be named by the number of their row in the tilemap.
		void autoAddAnimations(sf::Image spritemap, int width, int height, vector<int> delays, vector<string> names = vector<string>())
		{
			//Get a list of textures from the spritemap
			vector<sf::Texture> textures;
			textures = engine::SliceSpritemap(spritemap, width, height);

			//For each row in the spritemap
			for (size_t i = 0, j = 0; i < textures.size(); j++)
			{
				//Frame buffer to add to animation
				vector<sf::Texture> animationSlice;
				//For each column in the spritemap
				for (i; i < textures.size() / (spritemap.getSize().x / width); i++)
				{
					//Add the next texture to the buffer 
					animationSlice.push_back(textures[i]);
				}

				//Create new animation and add buffer frames to it
				engine::Animation newAnimation;
				newAnimation.Load(textures, delays);

				//If there are names in the names list add the animation with that
				if (names.size() < j)
					animations[names[j]] = newAnimation;
				else
					//If not name it the index of the row
					animations[to_string(j)] = newAnimation;
			}
		}
	};
}
