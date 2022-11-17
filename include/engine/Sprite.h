#pragma once
#include "ECSCore.h"
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

extern ECS ecs;

namespace engine
{
	//Sprite component
	struct Sprite
	{
		sf::Texture sprite;
		int layer = 0;
	};

	//Container for a single animation
	struct Animation
	{
		vector<sf::Texture> textures;
		vector<int> delays;
		int length = 0;
	};

	//Animator Component
	struct Animator
	{
		map<string, Animation> animations;

		string currentAnimation;
		int animationFrame = 0;
		bool repeatAnimation = false;
		bool playingAnimation = false;

		sf::Clock animationTimer;
	};

	//Animator system
	class AnimationSystem : public System
	{
	public:
		//Update every entity with relevant components
		void Update()
		{
			//For each entity that has the required components
			for (auto const& entity : entities)
			{
				//Get the relevant components from entity
				Animator& animator = ecs.getComponent<Animator>(entity);

				//If the entity is currently playing an animation
				if (animator.playingAnimation)
				{
					//If enough time (defined by animation) has passed
					if (animator.animationTimer.getElapsedTime().asMilliseconds() >= animator.animations[animator.currentAnimation].delays[animator.animationFrame])
					{
						AdvanceFrame(entity);
					}
				}
			}
		}

		//Advance to the next animation frame of current animation
		void AdvanceFrame(Entity entity)
		{
			//Get the relevant components from entity
			Animator& animator = ecs.getComponent<Animator>(entity);
			Sprite& sprite = ecs.getComponent<Sprite>(entity);

			//Change GameObject texture
			sprite.sprite = animator.animations[animator.currentAnimation].textures[animator.animationFrame];

			animator.animationTimer.restart();
			animator.animationFrame++;

			//If end of animation has been reached go to start or end animation
			if (animator.animationFrame >= animator.animations[animator.currentAnimation].length)
			{
				animator.animationFrame = 0;
				if (!animator.repeatAnimation)
				{
					animator.playingAnimation = false;
					animator.currentAnimation = "";
				}
			}
		}
	};

	//Automatically slice and create animations from a spritemap with delays in ms and optional names.
	//Adds one animation per row of sprites in the spritemap. You must provide one delay per sprite in the delays vector. 
	//All sprites must have the same width and height.
	vector<Animation> AnimationsFromSpritemap(sf::Image spritemap, int width, int height, vector<int> delays)
	{
		//Get a list of textures from the spritemap
		vector<sf::Texture> textures;
		textures = engine::SliceSpritemap(spritemap, width, height);

		vector<Animation> newAnimations;

		if (delays.size() < textures.size())
		{
			cout << "Not enough delays for amount of frames!\nYou must provide one delay after each frame (even the last one).\n";
			return vector<Animation>();
		}

		//For each row in the spritemap
		for (size_t y = 0; y < floor(spritemap.getSize().y / height); y++)
		{
			//Frame buffer to add to animation
			vector<sf::Texture> animationSlice;
			vector<int> delaySlice;

			//For each column in the spritemap
			for (size_t x = 0; x < floor(spritemap.getSize().x / width); x++)
			{
				//Add the next texture to the buffer 
				animationSlice.push_back(textures[x + y]);
				delaySlice.push_back(delays[x + y]);
			}

			Animation newAnimation{
				.textures = animationSlice,
				.delays = delaySlice,
				.length = animationSlice.size()
			};

			newAnimations.push_back(newAnimation);
		}
		return newAnimations;
	}

	//Add animations to entity, optionally provide names for those animations, otherwise they will be numbered in order
	void AddAnimations(Entity entity, vector<Animation> animations, vector<string> names = vector<string>()) 
	{
		Animator& animator = ecs.getComponent<Animator>(entity);

		//For each animation to add
		for (size_t i = 0; i < animations.size(); i++)
		{
			//Add the animation indexed by given name or number order
			if (names.size() > i) 
				animator.animations.insert({names[i], animations[i]});
			else
				animator.animations.insert({ to_string(i), animations[i] });
		}
	}

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
		for (size_t x = 0; x < (int)spritemap.getSize().x - width + 1; x += width)
		{
			for (size_t y = 0; y < (int)spritemap.getSize().y - height + 1; y += height)
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