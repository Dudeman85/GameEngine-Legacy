#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

namespace engine 
{
	//Animation class, contains one animation per instance
	class Animation
	{
	public:
		vector<int> delays;
		vector<sf::Texture> textures;
		int length = 0;

		//Load Animation From texture vector with equal delay intervals in ms
		void Load(vector<sf::Texture> frames, int delay) 
		{
			textures = frames;
			for (size_t i = 0; i < frames.size(); i++)
			{
				delays.push_back(delay);
			}
			length = textures.size();
		}
		//Load Animation from texture vector with custom delay intervals vector in ms
		void Load(vector<sf::Texture> frames, vector<int> delay) 
		{
			textures = frames;

			for (size_t i = 0; i < frames.size(); i++)
			{
				delays.push_back(delay[i]);
			}
			length = textures.size();
		}
	};

	//Slices an image into equal sized textures of width and height in pixels.
	//Returns a vector of textures ordered left to right top to bottom 
	vector<sf::Texture> SliceSpritemap(sf::Image spritemap, int width, int height) 
	{
		//Throw warning if spritemap is not properly dimensioned
		if (spritemap.getSize().x % width != 0)
			cout << "Warning: Spritemap width is not a multiple of sprite width. Clipping may occur!\n";
		if (spritemap.getSize().y % height != 0)
			cout << "Warning: Spritemap height is not a multiple of sprite height. Clipping may occur!\n";

		vector<sf::Texture> slicedSpritemap;
		//Run through each full sprite in the spritemap
		//if the final row or column are not full, skip them
		for (size_t x = 0; x < spritemap.getSize().x - width + 1; x += width)
		{
			for (size_t y = 0; y < spritemap.getSize().y - height + 1; y += height)
			{
				//Copy sector of spritemap to new texture
				sf::Image slice;
				slice.create(width, height);
				slice.copy(spritemap, 0, 0, sf::IntRect(x, y, x + width, y + height), true);

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
		slice.create(x2 - x1, y2 - y1);
		slice.copy(spritemap, 0, 0, sf::IntRect(x1, y1, x2, y2), true);
		
		sf::Texture slicedTexture = sf::Texture();
		slicedTexture.loadFromImage(slice);
		
		return slicedTexture;
	}
}