#include "engine/Application.h"
//Create one instance of the ecs manager
ECS ecs;

int main()
{
    // New engine instance, contains basic systems
    engine::EngineLib engine;
    // Tiled Map object
    tmx::Map map;

    //Create a new entity
    Entity player = ecs.newEntity();

    // Adding sprite to player
    ecs.addComponent(player, engine::Sprite());

    // Adding transform to player
    ecs.addComponent(player, engine::Transform());

    // Loading texture
    // new sf texture
    sf::Image spritesheet = engine::LoadImage("texturemap1.png");

    //first sprite
    sf::Texture texture = engine::CustomSlice(spritesheet, 0, 0, 64, 64);

    // players sprite component reference
    engine::Sprite& spriteComponent = ecs.getComponent<engine::Sprite>(player); // OpenGL broken no need to worry
    
    // Sprite component texture
    spriteComponent.texture = texture;

    // set player position
    engine.transformSystem->setPosition(player, 100, 60); // OpenGL broken no need to worry

    // Window creation
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    //assets tmx load function
    //map.load("assets/demo.tmx");
    //map.load("assets/Tiled/maps/ColliderTestMap.tmx");
    //Check has map been loaded
    if (map.load("assets/Tiled/maps/ColliderTestMap.tmx"))
    {
        // Get Tiled map layers address
        const auto& layers = map.getLayers();
        // Go through all the layers in the map
        for (const auto& layer : layers)
        {
            // See if we found our Collider Layer
            if (layer->getType() == tmx::Layer::Type::Object)
            {
                // Switching from Layer to Object class
                const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
            }
        }
    }

    // Layers creation
   // MapLayer layerZero(map, 0);
   // MapLayer layerOne(map, 1);
   // MapLayer layerTwo(map, 2);
    // run the program as long as the window is open
    // Game loop
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Cyan);

        // Rendering
        engine.renderSystem->Update(window);
        // layer drawing
        //window.draw(layerZero);
       // window.draw(layerOne);
       // window.draw(layerTwo);


        // 4: end the current frame (swap buffers)
        window.display();
    } // End - while()  
    return 0;
}