#pragma once
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "Sprite.h"

using namespace std;

namespace engine
{
	class GameObject
	{
	private:
		map<string, engine::Animation> animations;
		sf::Sprite sprite;

	public:
		bool enabled = true;
		int x = 0;
		int y = 0;

		sf::Texture texture;

		GameObject(sf::Texture& _texture) : texture(_texture)
		{

		}

		void Render(sf::RenderWindow window)
		{
			sprite.setPosition(x, y);
			sprite.setTexture(texture);

			if (enabled)
				window.draw(sprite);
		}

		//Animation methods

		//Add an animation with custom delays in ms
		void AddAnimation(string name, vector<sf::Texture> textures, vector<int> delays)
		{
			engine::Animation newAnimation;
			newAnimation.Load(textures, delays);

			animations[name] = newAnimation;
		}
		//Add an animation with equal delays in ms
		void AddAnimation(string name, vector<sf::Texture> textures, int delays)
		{
			engine::Animation newAnimation;
			newAnimation.Load(textures, delays);

			animations[name] = newAnimation;
		}

		//Automatically slice and add animations from a spritemap with equal delays in ms and optional names.
		//Adds one animation per row of sprites in the spritemap and sets every frame delay to be the same. All sprites must have the same width and height.
		//Optionally give a vector of strings for animation names, otherwise they will be named by the number of their row in the tilemap.
		void AutoAddAnimations(sf::Image spritemap, int width, int height, int delays, vector<string> names = vector<string>())
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

		//Automatically slice and add animations from a spritemap with custom delays in ms and optional names.
		//Adds one animation per row of sprites in the spritemap and sets every frame delay to be the same. All sprites must have the same width and height.
		//Optionally give a vector of strings for animation names, otherwise they will be named by the number of their row in the tilemap.
		void AutoAddAnimations(sf::Image spritemap, int width, int height, vector<int> delays, vector<string> names = vector<string>())
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
