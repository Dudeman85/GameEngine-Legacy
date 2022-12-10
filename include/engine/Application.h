#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <vector>

// tmxlite includes
#include <tmxlite/Map.hpp>
#include <engine/SFMLOrthogonalLayer.h>
// ObjectGroup include for Box2d collision
#include <tmxlite/ObjectGroup.hpp>

//ECS modules
#include "ECSCore.h"
#include "Sprite.h"
#include "Transform.h"
#include "Gravity.h"

using namespace std;

extern ECS ecs;

namespace engine 
{
	class EngineLib
	{
	public:
		shared_ptr<AnimationSystem> animationSystem;
		shared_ptr<RenderSystem> renderSystem;
		shared_ptr<TransformSystem> transformSystem;
		shared_ptr<PhysicsSystem> physicsSystem;

		EngineLib()
		{
			//Register all default engine components here
			ecs.registerComponent<Sprite>();
			ecs.registerComponent<Transform>();
			ecs.registerComponent<Animator>();
			ecs.registerComponent<Rigidbody>();

			//Register all default engine systems here

			//Animation System
			animationSystem = ecs.registerSystem<AnimationSystem>();
			Signature animationSystemSignature;
			animationSystemSignature.set(ecs.getComponentId<Sprite>());
			animationSystemSignature.set(ecs.getComponentId<Animator>());
			ecs.setSystemSignature<AnimationSystem>(animationSystemSignature);

			//Render System
			renderSystem = ecs.registerSystem<RenderSystem>();
			Signature renderSystemSignature;
			renderSystemSignature.set(ecs.getComponentId<Sprite>());
			renderSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<RenderSystem>(renderSystemSignature);

			//Transform System
			transformSystem = ecs.registerSystem<TransformSystem>();
			Signature transformSystemSignature;
			transformSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<TransformSystem>(transformSystemSignature);
			
			//Physics System
			physicsSystem = ecs.registerSystem<PhysicsSystem>();
			Signature physicsSystemSignature;
			physicsSystemSignature.set(ecs.getComponentId<Rigidbody>());
			physicsSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<PhysicsSystem>(physicsSystemSignature);
		}
	};

	//Engine Resource Variables
	string levelPath = "levels/";
	string assetPath = "assets/";

	//Draws each layer of the tilemap to the render window
	void RenderTilemap(tmx::Map* tilemap, sf::RenderWindow* window)
	{
		for (size_t i = 0; i < tilemap->getLayers().size(); i++)
		{
			MapLayer mapLayer(*tilemap, i);
			window->draw(mapLayer);
		}
	}

	//Tilemap Variables
	int scale = 50;
	int mapWidth = 20;
	int mapHeight = 54;

	//Save 2D vector tilemap to file
	void SaveTilemap(vector<vector<uint8_t>> tilemap, string file)
	{
		//Create a new directory and tilemap save file
		filesystem::create_directory(levelPath);
		ofstream tilemapSave(levelPath + file, ios_base::out);

		//For each x and y of given tilemap
		for (int x = 0; x < tilemap.size(); x++)
		{
			for (int y = 0; y < tilemap[0].size(); y++)
			{
				//Write tilemap tile id to file
				tilemapSave << tilemap[x][y];
			}
			tilemapSave << endl;
		}

		cout << "Saved tilemap to " << levelPath + file << endl;
		tilemapSave.close();
	}

	//Load 2D vector tilemap from file
	vector<vector<uint8_t>> LoadTilemap(string file)
	{
		//Try to open a tilemap of given name
		ifstream tilemapSave(levelPath + file);
		//If failed, return blank tilemap
		if (tilemapSave.fail())
		{
			cout << "Failed to load tilemap " + file << endl;
			vector<vector<uint8_t>> tilemap(mapHeight, vector<uint8_t>(mapWidth, 1));
			return tilemap;
		}

		//Create a new tilemap vector and iterate through each row and column of saved tilemap
		vector<vector<uint8_t>> tilemap;
		for (string sRow; getline(tilemapSave, sRow);)
		{
			vector<uint8_t> row;
			for (int i = 0; i < sRow.length(); i++)
			{
				//Add tile to row
				row.push_back(sRow[i]);
			}
			//Add row to tilemap
			tilemap.push_back(row);
		}

		tilemapSave.close();
		cout << "Loaded tilemap from " << levelPath + file << endl;
		return tilemap;
	}

	//Load texture with error checking
	sf::Texture LoadTexture(string name)
	{
		sf::Texture texture;
		if (!texture.loadFromFile(assetPath + name))
		{
			printf("Load texture error");
		}
		return texture;
	}

	//Load texture with error checking
	sf::Image LoadImage(string name)
	{
		sf::Image image;
		if (!image.loadFromFile(assetPath + name))
		{
			printf("Load texture error");
		}
		return image;
	}

	//Returns true if any corner of sprite is overlapping a non zero position of the tilemap.
	//Assumes origin of sprite is in the center. Optional offset for shifting the hitbox.
	bool CheckTilemapCollision(sf::Sprite sprite, vector<vector<uint8_t>> tilemap, sf::Vector2f offset = sf::Vector2f(0, 0))
	{
		//Get position(center) width and height of collision box
		sf::Vector2f position = sprite.getPosition() + offset;
		int width = sprite.getGlobalBounds().width / 2;
		int height = sprite.getGlobalBounds().height / 2;

		//Check if in bounds
		if (position.x - width > 0 && position.x + width < mapWidth * scale && position.y + height < mapHeight * scale && position.y - height > 0)
			//Check if any corner is within the tilemap by indexing the tilemap with each corner position divided by tilemap pixel scale
			return (tilemap[(position.y - height) / scale][(position.x - width) / scale] != 0) ||
			(tilemap[(position.y - height) / scale][(position.x + width) / scale] != 0) ||
			(tilemap[(position.y + height) / scale][(position.x - width) / scale] != 0) ||
			(tilemap[(position.y + height) / scale][(position.x + width) / scale] != 0);

		//If not in bounds return true
		return true;
	}

	//integrating sound:

	class soundAmbient {        // The class
	public:              // Access specifier
		
		void playSound(string name);   // Method/function declaration
		
	};

	// Method/function definition outside the class
	void soundAmbient::playSound(string name) {
			
			cout << "Sound effect plays";

			}

	int main() {
		soundAmbient myObj;     // Create an object of soundAmbient
		myObj.playSound("spring-weather-1.wav");  // Call the method

		 
		return 0;
	}
	
}