#include "engine/Application.h"

//Create one instance of the ecs manager
ECS ecs;

int main()
{
    // New engine instance, contains basic systems
    engine::EngineLib engine;

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
    engine::Sprite& spriteComponent = ecs.getComponent<engine::Sprite>(player);
    
    // Sprite component texture
    spriteComponent.texture = texture;

    // set player position
    engine.transformSystem->setPosition(player, 100, 60);

    // Window creation
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
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
        engine.renderSystem->Render(window);


        // 4: end the current frame (swap buffers)
        window.display();
    } // End - while()  


    return 0;
}