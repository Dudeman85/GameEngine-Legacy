#include <iostream>
#include <fstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;

const string levelPath = "levels/";
const string assetPath = "assets/";
const int scale = 50;
const int mapWidth = 20;
const int mapHeight = 54;

//Save 2D vector tilemap to file
void SaveTilemap(vector<vector<uint8_t>> tilemap, string file)
{
	filesystem::create_directory(levelPath);
	ofstream tilemapSave(levelPath + file, ios_base::out);

	for (int x = 0; x < tilemap.size(); x++)
	{
		for (int y = 0; y < tilemap[0].size(); y++)
		{
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
	ifstream tilemapSave(levelPath + file);
	if (tilemapSave.fail())
	{
		cout << "Failed to load tilemap " + file << endl;
		vector<vector<uint8_t>> tilemap(mapHeight, vector<uint8_t>(mapWidth, 1));
		return tilemap;
	}

	vector<vector<uint8_t>> tilemap;
	for (string sRow; getline(tilemapSave, sRow);)
	{
		vector<uint8_t> row;
		for (int i = 0; i < sRow.length(); i++)
		{
			row.push_back(sRow[i]);
		}
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

//Returns true if any corner of sprite is overlapping a non zero position of the tilemap.
//Assumes origin of sprite is in the center. Optional offset for shifting the hitbox.
bool CheckTilemapCollision(sf::Sprite sprite, vector<vector<uint8_t>> tilemap, sf::Vector2f offset = sf::Vector2f(0, 0))
{
	sf::Vector2f position = sprite.getPosition() + offset;
	int width = sprite.getGlobalBounds().width / 2;
	int height = sprite.getGlobalBounds().height / 2;

	//Check if in bounds
	if (position.x - width > 0 && position.x + width < mapWidth * scale && position.y + height < mapHeight * scale && position.y - height > 0)
		//Check if any corner is within the tilemap
		return (tilemap[(position.y - height) / scale][(position.x - width) / scale] != 0) ||
		(tilemap[(position.y - height) / scale][(position.x + width) / scale] != 0) ||
		(tilemap[(position.y + height) / scale][(position.x - width) / scale] != 0) ||
		(tilemap[(position.y + height) / scale][(position.x + width) / scale] != 0);

	return true;
}
