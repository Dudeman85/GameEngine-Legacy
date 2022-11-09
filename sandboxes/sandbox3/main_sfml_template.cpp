
#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<string>
#include<iostream>
#include<ctime>
#include "engine/Application.h"
#include "engine/GameObject.h"
#include "engine/Application.h"
#include "engine/GameObject.h"
#include "engine/Sprite.h"
//assetit eri kansioista riippuen ollaanko debug vai release käännöksessä
#if defined (_DEBUG)
std::string ASSET_PATH = "C:/Users/viksterikap/Desktop/projekti/tiimi4/sandboxes/sandbox3/assets/";
#else
std::string ASSET_PATH = "assets/";
#endif
#include <box2d/box2d.h>

sf::Texture loadTexture(const std::string& fileName)
{
    sf::Texture texture;
    if (!texture.loadFromFile(ASSET_PATH + fileName))
    {
        // error...
        printf("Error loading image!\n");

    }
    return texture;
}

class GameObject
{
public:
    GameObject(float px, float py)
        :positionX(px), positionY(py)
    {}
    float getX() const {
        return positionX;
    }
    float getY() const {
        return positionY;
    }

    void move(float deltaX, float deltaY) {
        positionX += deltaX;
        positionY += deltaY;
    };

    void move1(float gammaX, float gammaY) {
        positionX += gammaX;
        positionY += gammaY;

    };



private:
    float positionX;
    float positionY;

};



bool sphereSphereCollisionCheck(const GameObject& o1, const GameObject& o2)
{
    float deltaX = o2.getX() - o1.getX();
    float deltaY = o2.getY() - o1.getY();
    float c = abs(deltaX * deltaX + deltaY * deltaY);
    //oletetaan että peliobjektit ovat 1:n kokoisia, eli:
    // r1=0.5, r2 =0.5 -> ==1
    float r1PlusR2 = 1.0f;
    return c < r1PlusR2;
}




int main()
{
    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);
    // create the window
    sf::RenderWindow window(sf::VideoMode(768, 640), "My window");
    std::vector<sf::Texture> textures =
    {
        loadTexture("ground2.png"),
        loadTexture("ground.png"),
        loadTexture("Jill.png"),
        loadTexture("Ghost.png"),
        loadTexture("alumiini.png"),
        loadTexture("sun.png"),
        loadTexture("texturemap1.png"),
    };
  
    sf::Clock frameTimer;

    int points = 0;
    float enemySpawnTimer = 0.f;
    float enemySpawnTimerMax = 1000.f;
    int maxEnemies = 5;

    //8-bit value to describe, what kind of game object is in location x,y
    //mappää 
    const int SCALE = 64.0f;
    std::vector<std::vector<uint8_t>>map = {
        {0,0,0,0,0,0,0,0,0,0,1,0},
        {0,1,0,1,0,0,1,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,1,0,0,0,1},
        {0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,1,0},
        {0,0,0,0,0,0,1,0,0,0,0,0}
    };

    std::vector<GameObject> players = {
        GameObject(0,0)
    };

    std::vector<GameObject> enemies = {
        GameObject(6,4),
        GameObject(4,9),
        GameObject(9,6),
        GameObject(2,7)
    };

    std::vector<GameObject> platforms = {
        GameObject(5,5),
        GameObject(6,5),
        GameObject(2,6),
        GameObject(1,6),
        GameObject(9,7),
        GameObject(8,7)
    };

    std::vector<GameObject> platforms2 = {

        GameObject(0,10)
    };

    std::vector<GameObject> pisteet = {
        GameObject(11,9)
    };


    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float deltaTime = frameTimer.getElapsedTime().asSeconds();
        frameTimer.restart();

        // clear the window with black color
        window.clear(sf::Color::Blue);


        //1. read input
        float deltaX = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            deltaX -= 3.0 * deltaTime;


        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            deltaX += 3.0 * deltaTime;

        }
        float deltaY = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            deltaY -= 3.0 * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            deltaY += 3.0 * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        srand(time(NULL));
        int randomizer = 0 + rand() % 2 + -1;
        int randomizer1 = 0 + rand() % 3 + -2;

        //2. Move game objects according to input(update)
        players[0].move(deltaX, deltaY);


        enemies[0].move(deltaX * randomizer, deltaY * randomizer);
        enemies[1].move(deltaX * randomizer1, deltaY * randomizer1);
        enemies[3].move(deltaX * randomizer, deltaY * randomizer);
       

        
        // Enemy spawner
        if (enemies.size() < maxEnemies)
        {
            if (enemySpawnTimer >= enemySpawnTimerMax)
            {
                enemySpawnTimer = 0.f;
            }

            else
            {
                enemySpawnTimer += 1.f;
            }
        }





        //check collisions
        for (size_t playerIndex = 0; playerIndex < players.size(); ++playerIndex)
        {
            for (size_t enemyIndex = 0; enemyIndex < enemies.size(); ++enemyIndex)
            {
                if (sphereSphereCollisionCheck(players[playerIndex], enemies[enemyIndex]))
                {
                    printf("Player %d collides with enemy: %d\n", (int)playerIndex, (int)enemyIndex);
                    window.close();
                }
            }
        }
        for (size_t playerIndex = 0; playerIndex < players.size(); ++playerIndex)
        {
            for (size_t PlatformIndex = 0; PlatformIndex < platforms.size(); ++PlatformIndex)
            {
                if (sphereSphereCollisionCheck(players[playerIndex], platforms[PlatformIndex]))
                {
                    printf("Player %d collides with platform: %d\n", (int)playerIndex, (int)PlatformIndex);
                    deltaX == 0.0;
                    deltaY == 0.0;
                    players[0].move(deltaX == 0.0, deltaY == 0.0);
                }
            }
        }
        for (size_t playerIndex = 0; playerIndex < players.size(); ++playerIndex)
        {
            for (size_t pisteIndex = 0; pisteIndex < pisteet.size(); ++pisteIndex)
            {
                if (sphereSphereCollisionCheck(players[playerIndex], pisteet[pisteIndex]))
                {
                    printf("Player %d gets point: %d\n", (int)playerIndex, (int)pisteIndex);

                }
            }
        }

        // draw everything here...


        //3. "render"
        // Piirrä ensin map ruudulle
        for (size_t y = 0; y < map.size(); ++y)
        {
            for (size_t x = 0; x < map[y].size(); ++x)
            {
                auto spriteType = map[y][x];
                //Tee sprite luokasta instanssi ja määritä spritelle piirrettävä tekstuuri
                sf::Sprite mySprite(textures[spriteType]);

                mySprite.setPosition(SCALE * x, SCALE * y);
                window.draw(mySprite);
            }
        }
        for (size_t i = 0; i < platforms.size(); ++i)
        {
            auto spriteType = 4;
            //Tee sprite luokasta instanssi ja määritä spritelle piirrettävä tekstuuri
            sf::Sprite mySprite(textures[spriteType]);
            auto& platform = platforms[i];
            mySprite.setPosition(SCALE * platform.getX(), SCALE * platform.getY());
            mySprite.setScale(1.0f, 1.0f);
            window.draw(mySprite);
        }

        for (size_t i = 0; i < platforms2.size(); ++i)
        {
            auto spriteType = 4;
            //Tee sprite luokasta instanssi ja määritä spritelle piirrettävä tekstuuri
            sf::Sprite mySprite(textures[spriteType]);
            auto& platform = platforms2[i];
            mySprite.setPosition(SCALE * platform.getX(), SCALE * platform.getY());
            mySprite.setScale(12.0f, -.2f);
            window.draw(mySprite);
        }

        for (size_t i = 0; i < players.size(); ++i)
        {
            auto spriteType = 2;
            //Tee sprite luokasta instanssi ja määritä spritelle piirrettävä tekstuuri
            sf::Sprite mySprite(textures[spriteType]);
            auto& player = players[i];
            mySprite.setPosition(SCALE * player.getX(), SCALE * player.getY());
            window.draw(mySprite);
        }
        for (size_t i = 0; i < enemies.size(); ++i)
        {
            auto spriteType = 3;
            //Tee sprite luokasta instanssi ja määritä spritelle piirrettävä tekstuuri
            sf::Sprite mySprite(textures[spriteType]);
            auto& enemy = enemies[i];
            mySprite.setPosition(SCALE * enemy.getX(), SCALE * enemy.getY());
            window.draw(mySprite);
        }

        for (size_t i = 0; i < pisteet.size(); ++i)
        {
            auto spriteType = 5;
            //Tee sprite luokasta instanssi ja määritä spritelle piirrettävä tekstuuri
            sf::Sprite mySprite(textures[spriteType]);
            auto& piste = pisteet[i];
            mySprite.setPosition(SCALE * piste.getX(), SCALE * piste.getY());
            window.draw(mySprite);
        }

        // end the current frame
        window.display();
    }


    return 0;
}
