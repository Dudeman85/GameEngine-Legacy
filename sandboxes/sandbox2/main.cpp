#include <SFML/Graphics.hpp>
#include <engine/Application.h>
#include <engine/GameObject.h>
#include <stdio.h>
#include <string>

// assetit eri kansiosta riippuen ollanko debug vai relese käännöksessä.
#if defined(_DEBUG)
std::string ASSET_PATH = "../assets/";
#else 
std::string ASSET_PATH = "assets/";
#endif

int main() {
	// Tee ikkuna ja sovellus
	sf::RenderWindow window(sf::VideoMode(800,600), "My window");
	engine::Application app({
		"image.png",
	}, &window);

	// Tee GameObject -tyyppinen peliobjekti ja hae sen sprite viittaus.
	sf::Sprite& player = app.spawn<engine::GameObject>(0)->getSprite();
	player.setPosition(400.0f,300.0f);

	// Run the program as long as the window is open
	while(app.shouldClose() == false) {
		// update and get delta time
		float deltaTime = app.update();

		// Read input
		float deltaX = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            deltaX -= 100.0 * deltaTime;
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            deltaX += 100.0 * deltaTime;
        }
        float deltaY = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            deltaY -= 100.0 * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            deltaY += 100.0 * deltaTime;
		}

		// Move game objects according to input (update)
		player.move(deltaX, deltaY);

		player.setScale(0.1f, 0.1f);
        //mySprite.setRotation(45.0f);
        //mySprite.rotate(360.0f * deltaTime);       
        //mySprite.setPosition(200.0f,100.0f);

		// Render
		app.render();
    }

    return 0;
}
