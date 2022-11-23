#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <vector>
#include <box2d/box2d.h>
#include "ECSCore.h"
#include "Sprite.h"

using namespace std;

extern ECS ecs;

namespace engine 
{
	class Engine
	{
	public:
		std::shared_ptr<AnimationSystem> animationSystem;
		std::shared_ptr<RenderSystem> renderSystem;
		Engine()
		{
			//Register all engine systems here

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
			ecs.setSystemSignature<RenderSystem>(renderSystemSignature);
		}
	};

	//Engine Resource Variables
	string levelPath = "levels/";
	string assetPath = "assets/";
	
	//Box2D
	b2Vec2 gravity(0.0f, 0.0f);
	b2World* world;
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	//Tilemap Variables
	int scale = 50;
	int mapWidth = 20;
	int mapHeight = 54;

	void Init(b2Vec2 gravity = b2Vec2_zero) 
	{
		gravity.Set(gravity.x, gravity.y);
		world->SetGravity(gravity);
	}

	void Update() 
	{
		//world->Step(timeStep, velocityIterations, positionIterations);
	}

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
	sf::Texture loadTexture(string name)
	{
		sf::Texture texture;
		if (!texture.loadFromFile(assetPath + name))
		{
			printf("Load texture error");
		}
		return texture;
	}

	//Load texture with error checking
	sf::Image loadImage(string name)
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
}