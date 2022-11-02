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
		vector<engine::Animation> animations;
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

			if(enabled)
				window.draw(sprite);
		}
		/*
		void AddSimpleAnimations(sf::Image spritemap, int delay) 
		{
			engine::Animation newAnimation;
			newAnimation.Load()

			animations.push_back();
		}*/
	};
}
