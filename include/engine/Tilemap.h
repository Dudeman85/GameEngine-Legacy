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

class Tilemap final
{
public:
	Tilemap();
	~Tilemap();
	
	void loadMap();
	void draw();

private:
	void initGLStuff(const tmx::Map&);
	std::shared_ptr<engine::Texture> loadTexture(const std::string&);

	std::vector<std::unique_ptr<MapLayer>> mapLayers;
	std::shared_ptr<MapLayer> collisionLayer;
	std::vector<std::shared_ptr<engine::Texture>> allTextures;
	
	engine::Shader* shader;
};
