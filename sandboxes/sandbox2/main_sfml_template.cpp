/*
* Itsenäistä opiskelua

#include <iostream>
#include <stdio.h>
#include <string>

// SFML includes
#include <SFML/Graphics.hpp>

int main()
{
    // Window
    sf::RenderWindow window(sf::VideoMode(990, 900), "Game Window");

    // Event triggers
    sf::Event event;

    // Suorita ohjelmaa niin pitkään,
    // kun ikkuna on auki
    while (window.isOpen())
    {

        // Käy läpi kaikki Eventit,
        // jotka tapahtuivat viime loopista
        while (window.pollEvent(event))
        {
            // Kutsutaan close komentoa
            // ja suljetaan ikkuna
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Päivitys

        // Renderöinti
        // Puhdista näyttö uudella värillä
        window.clear(sf::Color::Cyan);

        // Piirrä peli

        // Kerro ohjelmalle, että
        // näyttö ei piirrä enään
        window.display();

    }

    // Ohjelman loppu
    return 0;
}


// Pelaajalle ja Enemylle omat luokat,
// colliderin testausta varten.
// En saanut toimimaan kunnolla ja aika
// loppui kesken


// Pelaaja luokka
class Player
{
public:
    // Pelaaja luokan konstruktori, joka
    // saa muuttujien arvokseen float xPos, float yPos
    Player(float xPos, float yPos)
    {
        // Pelaajan X position
        // muuttujaan sijoitus
        float positionX = xPos;

        // Pelaajan Y position
        // muutttujaan sijoitus
        float positionY = yPos;
    }

    // Pelaajan getX() -funktio
    int getX() const
    {
        return positionX;
    }

    // Pelaajan getY() -funktio
    int getY() const
    {
        return positionY;
    }

    // Pelaajan move funktio, jossa sijoitetaan pelaajan
    // uuden position arvot float deltaX ja float deltaY
    // pelaajan muuttujiin positionX ja positionY
    void move(float deltaX, float deltaY)
    {
        positionX += deltaX;
        positionY += deltaY;
    }

private:
    // Pelaajan X positio muuttuja
    float positionX;
    // Pelaajan Y positio muuttuja
    float positionY;
};


// Enemy luokka
class Enemy
{
public:
    // Enemy luokan konstruktori, joka
    // saa muuttujien arvokseen float xPos, float yPos
    Enemy(float xPos, float yPos)
    {
        // Enemyn X position
        // muuttujaan sijoitus
        float positionX = xPos;

        // Enemyn Y position
        // muutttujaan sijoitus
        float positionY = yPos;
    }

    // Enemyn getX() -funktio
    int getX() const
    {
        return positionX;
    }

    // Enemyn getY() -funktio
    int getY() const
    {
        return positionY;
    }

private:
    // Enemyn X positio muuttuja
    float positionX;
    // Enemyn Y positio muuttuja
    float positionY;
};

*/
#include <iostream>

// SFML includes
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string>
#include <iostream>

// assetit eri kansiosta riippuen ollanko debug vai relese knnksess.
#if defined(_DEBUG)
std::string ASSET_PATH = "assets/";
#else 
std::string ASSET_PATH = "assets/";
#endif

sf::Texture loadTexture(const std::string& filename)
{
    sf::Texture texture;
    if (!texture.loadFromFile(ASSET_PATH + filename))
    {
        // error
        printf("Error loading image!\n");
    }
    return texture;
}


class GameObject
{
public:
    GameObject(float px, float py) : positionX(px), positionY(py) {}

    int getX() const {
        return positionX;
    }

    int getY() const {
        return positionY;
    }

    void move(float deltaX, float deltaY)
    {
        positionX += deltaX;
        positionY += deltaY;
    }

private:
    float positionX;
    float positionY;
};

bool sphereSphereCollisonCheck(const GameObject& o1, const GameObject& o2)
{
    float deltaX = o2.getX() - o1.getX();
    float deltaY = o2.getY() - o1.getY();
    float c = abs(deltaX * deltaX + deltaY * deltaY);
    // Oletetaan nyt, ett peliobjektit ovat 1:n kokoisia, eli
    // r1 = 0.5, r2 = 0.5 -> r1 +r2 == 1
    float r1PlusR2 = 1.0f;
    return c < r1PlusR2;
}

int main() {
    // Window creation
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    std::vector<sf::Texture> textures = {
        loadTexture("image.png"),
        loadTexture("PeepoBlankie.png"),
        loadTexture("Player.png"),
        loadTexture("Enemy.png"),
    };
    sf::Clock frameTimer;

    const int SCALE = 64.0f;
    std::vector<std::vector<uint8_t> > map = {
        {0,0,0,0},
        {0,1,0,1},
        {0,0,0,0},
        {0,0,0,0}
    };

    std::vector<GameObject> players = {
        GameObject(0,0)
    };

    std::vector<GameObject> enemies = {
        GameObject(4,2)
    };

    // Tee sprite luokasta instanssi ja mrit spritelle piirrettv tekstuuri
    sf::Sprite mySprite(textures[0]);

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
        float deltaTime = frameTimer.getElapsedTime().asSeconds();
        frameTimer.restart();
        // clear the window with black color
        window.clear(sf::Color::Cyan);

        // 1. Read input
        float deltaX = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            deltaX -= 10.0 * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            deltaX += 10.0 * deltaTime;
        }
        float deltaY = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            deltaY -= 10.0 * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            deltaY += 10.0 * deltaTime;
        }

        // Collision


        // 2. Move game objects according to input (update)
        players[0].move(deltaX, deltaY);
        std::cout << players[0].getY() << std::endl;

        for (size_t playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
            for (size_t enemyIndex = 0; enemyIndex < enemies.size(); ++enemyIndex) {
                if (sphereSphereCollisonCheck(players[playerIndex], enemies[enemyIndex])) {
                    printf("Player: %d collides with enemy %d\n", (int)playerIndex, int(enemyIndex));
                }
            }
        }

        //mySprite.setScale(0.1f, 0.1f);
        //mySprite.setRotation(45.0f);
        //mySprite.rotate(360.0f * deltaTime);       
        //mySprite.setPosition(200.0f,100.0f);

        // 3. "Render"
        // Piirr sprite ruudulle
        //window.draw(mySprite);
        for (size_t y = 0; y < map.size(); ++y)
        {
            for (size_t x = 0; x < map[y].size(); ++x)
            {
                auto spriteType = map[y][x];
                sf::Sprite mySprite(textures[spriteType]);
                mySprite.setPosition(SCALE * x, SCALE * y);
                window.draw(mySprite);
            }
        }
        for (size_t i = 0; i < players.size(); ++i)
        {
            auto spriteType = 2;
            sf::Sprite mySprite(textures[spriteType]);
            auto& player = players[i];
            mySprite.setPosition(SCALE * player.getX(), SCALE * player.getY());
            window.draw(mySprite);
        }

        for (size_t i = 0; i < enemies.size(); ++i)
        {
            auto spriteType = 3;
            sf::Sprite mySprite(textures[spriteType]);
            auto& enemy = enemies[i];
            mySprite.setPosition(SCALE * enemy.getX(), SCALE * enemy.getY());
            window.draw(mySprite);
        }

        // 4: end the current frame (swap buffers)
        window.display();
    } // End - while()  


    return 0;

}