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
#pragma once

#include <vector>
#include <string>
#include <engine/MapLayer.h>
#include <engine/GL/Texture.h>
#include <engine/GL/Shader.h>
#include <engine/GL/Camera.h>

class Tilemap final
{
public:
	Tilemap(engine::Camera* cam);
	~Tilemap();
	
	void loadMap(const std::string ownMap);
	void draw();

	unsigned int checkCollision(float x, float y);

	tmx::Vector2u tileSize;
	glm::vec3 TilemapPos;
private:
	void initGLStuff(const tmx::Map&);
	std::shared_ptr<engine::Texture> loadTexture(const std::string&);

	//A 2D vector of tile IDs used for simple tile collision checking
	std::vector<std::vector<unsigned int>> collisionLayer;

	tmx::FloatRect bounds;

	std::vector<std::unique_ptr<MapLayer>> mapLayers;
	std::vector<std::shared_ptr<engine::Texture>> allTextures;
	
	engine::Shader* m_shader;
	engine::Camera* camera;
};
