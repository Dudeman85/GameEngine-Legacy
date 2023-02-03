#if 0

#include "engine/Application.h"
//Create one instance of the ecs manager
ECS ecs;


int main()
{
    setlocale(LC_ALL, "");

    /*
    ECS test
    -----------------------------------------------
     // New engine instance, contains basic systems
    engine::EngineLib engine;
     //Create a new entity
    Entity player = ecs.newEntity();
    // Adding sprite to player
    ecs.addComponent(player, engine::Sprite());
    // Adding transform to player
    ecs.addComponent(player, engine::Transform());// Loading texture
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
    // Rendering
        engine.renderSystem->Update(window);
    ----------------------------------------------------------------------------------------
    */

    // List for Colliders
    std::vector<sf::FloatRect> colliders;

    // Tiled Map class
    tmx::Map map;

    // SFML window creation
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // tmx load function for Tiled assets
    //map.load("assets/demo.tmx");
   // map.load("assets/Tiled/maps/ColliderTestMap.tmx");

    // See has the Tiled map been loaded
    if (map.load("assets/Tiled/maps/ColliderTestMap.tmx"))
    {
        // Debug print
        std::cout << "Tiled versio > " << map.getVersion().upper << "," << map.getVersion().lower << std::endl;

        // Get Tiled maps address
        const auto& layers = map.getLayers();

        // Debug print
        std::cout << "Mapilla on > " << layers.size() << " layeriä" << std::endl;

        // Go through all the layers
        for (const auto& layer : layers)
        {
            // Debug prints
            std::cout << "Layeri löydetty > " << layer->getName() << std::endl;
            std::cout << " Layerin tyyppi > " << int(layer->getType()) << std::endl;

            // See if we have found object layer
            if (layer->getType() == tmx::Layer::Type::Object)
            {
                // Place Tiled layer objects address to object variable
                const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();

                // Debug print
                std::cout << "Löytyi > " << objects.size() << " objektia layeristä" << std::endl;

                // Go through the objects in object layer
                for (const auto& object : objects)
                {
                    // Debug print
                    std::cout << "Objekti" << object.getUID() << ", " << object.getName() << std::endl;

                    // All colliders AABB
                    tmx::FloatRect rect = object.getAABB();

                    // store colliders into library
                    colliders.push_back(sf::FloatRect(rect.left, rect.height, rect.top, rect.width));
                }
            }

            const auto& tilesets = map.getTilesets();

            for (const auto& tileset : tilesets)
            {
                //read out tile set properties, load textures etc...
            }
        }
    } 

    // Layers creation
    MapLayer layerZero(map, 0);
    MapLayer layerOne(map, 1);
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

        
        // layer drawing
        window.draw(layerZero);
        window.draw(layerOne);
       // window.draw(layerTwo);
        // 4: end the current frame (swap buffers)
        window.display();
    } // End - while()  
    return 0;
}
#endif

#include "engine/Application.h"

int main()
{
    // Tiled Map class
    tmx::Map map;

    // Download Tiled map from file
    map.load("CollidersTestMap.tmx");

    // Finding the collider-layer

}