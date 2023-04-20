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

#include <tmxlite/Map.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/Tilemap.h>
#include <engine/GL/Shader.h>
#include <engine/GL/Texture.h>
#include <engine/MapLayer.h>

#include <tmxlite/TileLayer.hpp>

Tilemap::Tilemap()
{
	shader = new engine::Shader("vertexShader.glsl", "fragmentShader.glsl");
}

Tilemap::~Tilemap()
{
}

void Tilemap::draw()
{
	shader->use();
	for (const auto& layer : mapLayers)
	{
		layer->draw();
	}
}

void Tilemap::loadMap()
{
	tmx::Map map;
	map.load("assets/demo.tmx");

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
				mapLayers.emplace_back(std::make_unique<MapLayer>(map, i, allTextures));
			}
		}
	}
}

unsigned int Tilemap::checkCollision(float x, float y)
{
	return collisionLayer[x / tileSize.x][y / tileSize.y];
}

void Tilemap::initGLStuff(const tmx::Map& map)
{
	auto m_projectionMatrix = glm::ortho(0.f, 800.f, 600.f, 0.f, -0.1f, 100.f);

	shader->use();
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "u_projectionMatrix"), 1, GL_FALSE, &m_projectionMatrix[0][0]);

	//we'll make sure the current tile texture is active in 0, 
	//and lookup texture is active in 1 in MapLayer::draw()
	glUniform1i(glGetUniformLocation(shader->ID, "u_tileMap"), 0);
	glUniform1i(glGetUniformLocation(shader->ID, "u_lookupMap"), 1);


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
	return std::make_shared<engine::Texture>("assets/images/tilemap/tileset.png", GL_NEAREST, false);
}