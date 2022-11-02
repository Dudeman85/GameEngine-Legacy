#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

namespace engine 
{
	vector<sf::Texture> SliceSpritemap(sf::Texture spritemap, int width, int height) 
	{
		vector<sf::Texture> slicedSpritemap;
		for (size_t x = 0; x < spritemap.getSize().x / width; x++)
		{
			for (size_t y = 0; y < spritemap.getSize().y / height; y++)
			{
				slicedSpritemap.push_back(spritemap);
			}
		}
	}
}