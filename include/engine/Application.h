#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdio.h>
#include <vector>
#include <chrono>

//ECS modules
#include "ECSCore.h"
#include "Sprite.h"
#include "Transform.h"
#include "Gravity.h"

//Other engine libs
#include <engine/GL/Window.h>

using namespace std;

extern ECS ecs;

namespace engine 
{
	//A class to store all the default engine modules
	class EngineLib
	{
	public:
		double deltaTime = 0;

		shared_ptr<AnimationSystem> animationSystem;
		shared_ptr<RenderSystem> renderSystem;
		shared_ptr<TransformSystem> transformSystem;
		shared_ptr<PhysicsSystem> physicsSystem;

		EngineLib()
		{
			//Make sure OpenGL context has been created
			assert(OPENGL_INITIALIZED && "OpenGL has not been initialized! Create a window, or manually create the OpenGL context before initializing EngineLib!");

			//Init time
			lastFrame = chrono::high_resolution_clock::now();

			//Register all default engine components here
			ecs.registerComponent<Sprite>();
			ecs.registerComponent<Transform>();
			ecs.registerComponent<Animator>();
			ecs.registerComponent<Rigidbody>();

			//Register all default engine systems here

			//Transform System
			transformSystem = ecs.registerSystem<TransformSystem>();
			Signature transformSystemSignature;
			transformSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<TransformSystem>(transformSystemSignature);

			//Render System
			renderSystem = ecs.registerSystem<RenderSystem>();
			Signature renderSystemSignature;
			renderSystemSignature.set(ecs.getComponentId<Sprite>());
			renderSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<RenderSystem>(renderSystemSignature);

			//Animation System
			animationSystem = ecs.registerSystem<AnimationSystem>();
			Signature animationSystemSignature;
			animationSystemSignature.set(ecs.getComponentId<Sprite>());
			animationSystemSignature.set(ecs.getComponentId<Animator>());
			ecs.setSystemSignature<AnimationSystem>(animationSystemSignature);

			//Physics System
			physicsSystem = ecs.registerSystem<PhysicsSystem>();
			Signature physicsSystemSignature;
			physicsSystemSignature.set(ecs.getComponentId<Rigidbody>());
			physicsSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<PhysicsSystem>(physicsSystemSignature);
		}

		//Updates all default engine systems, calculates and returns delta time
		double Update(Camera* cam)
		{
			//Update engine systems
			transformSystem->Update();
			renderSystem->Update(cam);
			animationSystem->Update(deltaTime);

			//Calculate Delta Time
			chrono::time_point thisFrame = chrono::high_resolution_clock::now();
			chrono::duration<double> duration = thisFrame - lastFrame;
			deltaTime = duration.count();
			lastFrame = thisFrame;

			return deltaTime;
		}
	
	private:
		chrono::time_point<chrono::high_resolution_clock> lastFrame;
	};

	//Engine Resource Variables
	string levelPath = "levels/";
	string assetPath = "assets/";


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

}