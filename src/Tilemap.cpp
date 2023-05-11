/*********************************************************************
Matt Marchant 2016
http://trederia.blogspot.com

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/
#define _USE_MATH_DEFINES
#include <engine/Tilemap.h>
#include <glm/gtc/matrix_transform.hpp>
#include <tmxlite/Map.hpp>
#include <engine/GL/Shader.h>
#include <engine/GL/Texture.h>
#include <engine/MapLayer.h>
#include <cassert>
#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Property.hpp>

Tilemap::Tilemap(engine::Camera* cam)
{
	m_shader = new engine::Shader("vertexShader.glsl", "fragmentShader.glsl");
	camera = cam;
	collisionLayer = std::vector<std::vector<unsigned int>>();
	position = glm::vec3(0);
}

Tilemap::~Tilemap()
{
}

void Tilemap::draw()
{
	m_shader->use();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::rotate(model, (float)M_PI, glm::vec3(1.0f, 0.0f, 0.0f));

	unsigned int modelLoc = glGetUniformLocation(m_shader->ID, "u_modelMatrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	unsigned int viewLoc = glGetUniformLocation(m_shader->ID, "u_viewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	//Give the shader the projection matrix
	unsigned int projLoc = glGetUniformLocation(m_shader->ID, "u_projectionMatrix");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	for (const auto& layer : mapLayers)
	{
		layer->draw();
	}
}

void Tilemap::loadMap(const std::string ownMap)
{
	tmx::Map map;
	//map.load("assets/demo.tmx");
	map.load(ownMap);

	//create shared resources, shader and tileset textures
	initGLStuff(map);


	bounds = map.getBounds();
	tileSize = map.getTileSize();

	//create a drawable object for each tile layer
	const auto& layers = map.getLayers();
	for (auto i = 0u; i < layers.size(); ++i)
	{
		if (layers[i]->getType() == tmx::Layer::Type::Tile)
		{
			//If the layer is a collision layer
			if (layers[i]->getName() == "collider")
			{
				//Resize the collision map
				collisionLayer.resize(map.getTileCount().x);
				for (int i = 0; i < collisionLayer.size(); i++)
				{
					collisionLayer[i].resize(map.getTileCount().y);
				}

				//Get the tile IDs
				auto& tiles = layers[i]->getLayerAs<tmx::TileLayer>().getTiles();

				//Transfer the tile IDs to the 2D collision vector
				for (int y = 0; y < collisionLayer[0].size(); y++)
				{
					for (int x = 0; x < collisionLayer.size(); x++)
					{
						collisionLayer[x][y] = tiles[(y * collisionLayer.size()) + x].ID;
					}
				}
				//Print for debug
				for (int y = 0; y < collisionLayer[0].size(); y++)
				{
					for (int x = 0; x < collisionLayer.size(); x++)
					{
						printf("%d ", (int)collisionLayer[x][y]);
					}
					printf("\n");
				}

			}
			else
			{
				// Get the custom property from Tiled Layer and place
				// int zOrder variable
				// TODO: Figure how to get custom property in tmxlite
				//auto zOrderProperty = layers[i]
				mapLayers.emplace_back(std::make_unique<MapLayer>(map, i, allTextures));
			}
		}
	}
}

//Returns the collision layers tile ID at x and y
unsigned int Tilemap::checkCollision(float x, float y)
{
	if (collisionLayer.empty())
		return 0;

	int xIndex = floor((x - position.x) / tileSize.x);
	int yIndex = floor((-y + position.y) / tileSize.y);

	//Check out of bounds
	if (abs(xIndex) >= collisionLayer.size() || abs(yIndex) >= collisionLayer[0].size() || y >= position.y || x <= position.x)
		return 0;

	return collisionLayer[xIndex][yIndex];
}

void Tilemap::initGLStuff(const tmx::Map& map)
{
	m_shader->use();

	//we'll make sure the current tile texture is active in 0, 
	//and lookup texture is active in 1 in MapLayer::draw()
	glUniform1i(glGetUniformLocation(m_shader->ID, "u_tileMap"), 0);
	glUniform1i(glGetUniformLocation(m_shader->ID, "u_lookupMap"), 1);


	const auto& tilesets = map.getTilesets();
	for (const auto& ts : tilesets)
	{
		auto texture = loadTexture(ts.getImagePath());
		allTextures.push_back(texture);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
}

std::shared_ptr<engine::Texture> Tilemap::loadTexture(const std::string& path)
{
	return std::make_shared<engine::Texture>(path.c_str(), GL_NEAREST, false);
}