#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

namespace engine 
{
	class Animation
	{
	public:
		vector<int> delays;
		vector<sf::Texture> textures;

		//Load Animation From texture vector with equal delay intervals in ms
		void Load(vector<sf::Texture> frames, int delay) 
		{
			textures = frames;
			for (size_t i = 0; i < frames.size(); i++)
			{
				delays.push_back(delay);
			}
		}
		//Load Animation from texture vector with custom delay intervals vector in ms
		void Load(vector<sf::Texture> frames, vector<int> delay) 
		{
			textures = frames;

			for (size_t i = 0; i < frames.size(); i++)
			{
				if (i > delay.size())
					delays.push_back(delay[delay.size() - 1]);
				delays.push_back(delay[i]);
			}
		}
	};

	//Slices an image into equal sized textures of width and height in pixels.
	//Returns a vector of textures ordered left to right top to bottom 
	vector<sf::Texture> SliceSpritemap(sf::Image spritemap, int width, int height) 
	{
		vector<sf::Texture> slicedSpritemap;
		for (size_t x = 0; x < spritemap.getSize().x; x += width)
		{
			for (size_t y = 0; y < spritemap.getSize().y; y += height)
			{
				sf::Image slice;
				slice.copy(spritemap, 0, 0, sf::IntRect(x, y, x + width, y + height));

				sf::Texture slicedTexture; 
				slicedTexture.loadFromImage(slice);

				slicedSpritemap.push_back(slicedTexture);
			}
		}

		return slicedSpritemap;
	}

	//Gets a subregion of an image by pixel coordinates from top-left to bottom-right
	//Return sliced texture
	sf::Texture CustomSlice(sf::Image spritemap, int x1, int y1, int x2, int y2) 
	{
		sf::Image slice;
		slice.copy(spritemap, 0, 0, sf::IntRect(x1, y1, x2, y2));

		sf::Texture slicedTexture;
		slicedTexture.loadFromImage(slice);

		return slicedTexture;
	}
}