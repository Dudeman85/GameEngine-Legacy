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

#include "Game.hpp"
#include "GLCheck.hpp"

//#include <glm/gtc/matrix_transform.hpp> Engine2

#include <tmxlite/Map.hpp>
//#include "engine/GL/Texture.h"
#include "engine/GL/Shader.h"
#include "engine/GL/Texture.h"


#include <cassert>
#include <array>

namespace
{
    bool running = true;
    const float timePerFrame = 1.f / 60.f;

#include "Shader.inl"
}

Game::Game()
{
    m_shader = engine::Shader("vertexShader.glsl", "fragmentShader.glsl");
}

Game::~Game()
{    
    for(auto t : m_allTextures)
    {
        delete &t;
    }
}
/*
//public
void Game::run(SDL_Window* window)
{
    assert(window);
    
    if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
    {
        std::cout << "Failed to load OpenGL functions" << std::endl;
        return;
    }
    loadMap();
        
    Uint32 lastTime = SDL_GetTicks();
    float elapsed = static_cast<float>(lastTime) / 1000.f;
    while(running)
    {
        while(elapsed > timePerFrame)
        {
            doEvents();
            update(timePerFrame);
            elapsed -= timePerFrame;
        }
        
        lastTime = SDL_GetTicks() - lastTime;
        elapsed += static_cast<float>(lastTime) / 1000.f;
        draw(window);
    }
}

//private
void Game::doEvents()
{
    SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        if(evt.type == SDL_QUIT) running = false;
        
        if(evt.type == SDL_KEYUP)
        {
            switch(evt.key.keysym.sym)
            {
            default:break;
            case SDLK_ESCAPE: running = false; break;
            }
        }
    }
}

void Game::update(float dt)
{

}
*/
void Game::draw()
{
    m_shader.use();
    

}


void Game::loadMap()
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


void Game::initGLStuff(const tmx::Map& map)
{
    //m_projectionMatrix = glm::ortho(0.f, 800.f, 600.f, 0.f, -0.1f, 100.f); Engine2
    
    m_shader.use();
    //glCheck(glUniformMatrix4fv(glGetUniformLocation(m_shader, "u_projectionMatrix"), 1, GL_FALSE, &m_projectionMatrix[0][0])); Engine2
    
    //we'll make sure the current tile texture is active in 0, 
    //and lookup texture is active in 1 in MapLayer::draw()
    glCheck(glUniform1i(glGetUniformLocation(m_shader.ID, "u_tileMap"), 0));
    glCheck(glUniform1i(glGetUniformLocation(m_shader.ID, "u_lookupMap"), 1));
    
    
    const auto& tilesets = map.getTilesets();
    for(const auto& ts : tilesets)
    {
        auto texture = loadTexture(ts.getImagePath());
        m_allTextures.push_back(texture);
    }
    
    
    
    glCheck(glClearColor(0.6f, 0.8f, 0.92f, 1.f));
    glCheck(glEnable(GL_BLEND));
    glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glCheck(glBlendEquation(GL_FUNC_ADD));
}

// SDL texture loading

std::shared_ptr<engine::Texture> Game::loadTexture(const std::string& path)
{    
    //m_tileTextures.push_back(new Texture(...));
    return std::make_shared<engine::Texture>("assets/images/tilemap", GL_LINEAR);
}











