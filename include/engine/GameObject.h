#pragma once
#include <SFML/Graphics.hpp>

namespace engine
{
	class GameObject
	{
	public:
		sf::Texture texture;

		GameObject(sf::Texture& _texture) : texture(_texture)
		{

		}
	};
}
