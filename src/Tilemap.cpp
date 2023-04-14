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

#include <engine/Tilemap.h>

#include <glm/gtc/matrix_transform.hpp>

#include <tmxlite/Map.hpp>
//#include "engine/GL/Texture.h"
#include <engine/GL/Shader.h>
#include <engine/GL/Texture.h>
#include <engine/MapLayer.h>


#include <cassert>
#include <array>

namespace
{
    bool running = true;
    const float timePerFrame = 1.f / 60.f;

}

TileMap::TileMap()
{
    m_shader = new engine::Shader("vertexShader.glsl", "fragmentShader.glsl");
}

TileMap::~TileMap()
{    
}

void TileMap::draw()
{
    glClearColor(0.5, 0, 0.5, 1);
    (glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));

    m_shader->use();
    for (const auto& layer : m_mapLayers)
    {
        layer->draw();
    }
  

}

void TileMap::loadMap()
{
    tmx::Map map;
    map.load("assets/demo.tmx");    
    
    //create shared resources, shader and tileset textures
    initGLStuff(map);

    //create a drawable object for each tile layer
    const auto& layers = map.getLayers();
    for(auto i = 0u; i < layers.size(); ++i)
    {
        if(layers[i]->getType() == tmx::Layer::Type::Tile)
        {
            m_mapLayers.emplace_back(std::make_unique<MapLayer>(map, i, m_allTextures));
        }
    }
}


void TileMap::initGLStuff(const tmx::Map& map)
{
    auto m_projectionMatrix = glm::ortho(0.f, 800.f, 600.f, 0.f, -0.1f, 100.f);
    
    m_shader->use();
    glUniformMatrix4fv(glGetUniformLocation(m_shader->ID, "u_projectionMatrix"), 1, GL_FALSE, &m_projectionMatrix[0][0]);
    
    //we'll make sure the current tile texture is active in 0, 
    //and lookup texture is active in 1 in MapLayer::draw()
    (glUniform1i(glGetUniformLocation(m_shader->ID, "u_tileMap"), 0));
    (glUniform1i(glGetUniformLocation(m_shader->ID, "u_lookupMap"), 1));
    
    
    const auto& tilesets = map.getTilesets();
    for(const auto& ts : tilesets)
    {
        auto texture = loadTexture(ts.getImagePath());
        m_allTextures.push_back(texture);
    }
    
    
    
    (glClearColor(0.6f, 0.8f, 0.92f, 1.f));
    (glEnable(GL_BLEND));
    (glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    (glBlendEquation(GL_FUNC_ADD));
}

// SDL texture loading

std::shared_ptr<engine::Texture> TileMap::loadTexture(const std::string& path)
{    
    //m_tileTextures.push_back(new Texture(...));
    return std::make_shared<engine::Texture>("assets/images/tilemap/tileset.png", GL_NEAREST, false);
}











