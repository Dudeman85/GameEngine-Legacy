#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

namespace engine 
{
	//
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
}