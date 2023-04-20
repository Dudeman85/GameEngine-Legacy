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
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace
{
	bool running = true;
	const float timePerFrame = 1.f / 60.f;

}

Tilemap::Tilemap(engine::Camera* cam)
{
	m_shader = new engine::Shader("vertexShader.glsl", "fragmentShader.glsl");
	camera = cam;
}

Tilemap::~Tilemap()
{
}

void Tilemap::draw()
{
	m_shader->use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, (float)M_PI , glm::vec3(1.0f, 0.0f, 0.0f));

	unsigned int modelLoc = glGetUniformLocation(m_shader->ID, "u_modelMatrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	unsigned int viewLoc = glGetUniformLocation(m_shader->ID, "u_viewMatrix");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	//Give the shader the projection matrix
	unsigned int projLoc = glGetUniformLocation(m_shader->ID, "u_projectionMatrix");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	for (const auto& layer : m_mapLayers)
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

	//create a drawable object for each tile layer
	const auto& layers = map.getLayers();
	for (auto i = 0u; i < layers.size(); ++i)
	{

		if (layers[i]->getType() == tmx::Layer::Type::Tile)
		{
			m_mapLayers.emplace_back(std::make_unique<MapLayer>(map, i, m_allTextures));
		}
	}
}

void Tilemap::initGLStuff(const tmx::Map& map)
{
	m_shader->use();

	//we'll make sure the current tile texture is active in 0, 
	//and lookup texture is active in 1 in MapLayer::draw()
	(glUniform1i(glGetUniformLocation(m_shader->ID, "u_tileMap"), 0));
	(glUniform1i(glGetUniformLocation(m_shader->ID, "u_lookupMap"), 1));


	const auto& tilesets = map.getTilesets();
	for (const auto& ts : tilesets)
	{
		auto texture = loadTexture(ts.getImagePath());
		m_allTextures.push_back(texture);
	}

	(glEnable(GL_BLEND));
	(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	(glBlendEquation(GL_FUNC_ADD));
}

std::shared_ptr<engine::Texture> Tilemap::loadTexture(const std::string& path)
{
	return std::make_shared<engine::Texture>("assets/images/tilemap/tileset.png", GL_NEAREST, false);
}











