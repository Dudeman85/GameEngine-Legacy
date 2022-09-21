#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<string>

//assetit eri kansioista riippuen ollaanko debug vai release k‰‰nnˆksess‰
#if defined (_DEBUG)
std::string ASSET_PATH = "../assets/";
#else
std::string ASSET_PATH = "assets/";
#endif

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
        :positionX(px),positionY(py)
    {}
    float getX() const {
        return positionX;
    }
    float getY() const {
        return positionY;
    }

    void move(float deltaX, float deltaY){
        positionX += deltaX;
        positionY += deltaY;
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
    //oletetaan ett‰ peliobjektit ovat 1:n kokoisia, eli:
    // r1=0.5, r2 =0.5 -> ==1
    float r1PlusR2 = 1.0f;
    return c < r1PlusR2;
}

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    std::vector<sf::Texture> textures =
    {
        loadTexture("thumbnail.png"),
        loadTexture("thumbnail2.png"),
        loadTexture("Jill.png"),
        loadTexture("Ghost.png"),
    };


    sf::Clock frameTimer;
    

    //8-bit value to describe, what kind of game object is in location x,y
    //mapp‰‰ 
    const int SCALE = 64.0f;
    std::vector<std::vector<uint8_t>>map = {
        {0,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,0,1,0,1,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,1,0,0},
        {0,0,0,0,0,0,0,0,0,0}
    };

    std::vector<GameObject> players = {
        GameObject(0,0)
    };

    std::vector<GameObject> enemies = {
        GameObject(6,4)
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

        //2. Move game objects according to input(update)
        players[0].move(deltaX, deltaY);

        //check collisions
        for (size_t playerIndex = 0; playerIndex < players.size(); ++playerIndex)
        {
            for (size_t enemyIndex = 0; enemyIndex < enemies.size(); ++enemyIndex)
            {
                if (sphereSphereCollisionCheck(players[playerIndex], enemies[enemyIndex]))
                {
                    printf("Player %d collides wiht enemy: %d\n", (int)playerIndex, (int)enemyIndex);
                }
            }
        }


        // draw everything here...
        //mySprite.setScale(1.0f, 1.0f);
        //mySprite.setRotation(45.0f);
        //mySprite.rotate(180.0f * deltaTime);
        //mySprite.setPosition(370.0f, -100.3f);
        //window.draw(mySprite);

        //3. "render"
        // Piirr‰ ensin map ruudulle
        for (size_t y = 0; y < map.size(); ++y)
        {
            for (size_t x = 0; x < map[y].size(); ++x)
            {
                auto spriteType = map[y][x];
                //Tee sprite luokasta instanssi ja m‰‰rit‰ spritelle piirrett‰v‰ tekstuuri
                sf::Sprite mySprite(textures[spriteType]);
                
                mySprite.setPosition(SCALE*x, SCALE*y);
                window.draw(mySprite);
            }
        }

        for (size_t i = 0; i < players.size(); ++i)
        {
            auto spriteType = 2;
            //Tee sprite luokasta instanssi ja m‰‰rit‰ spritelle piirrett‰v‰ tekstuuri
            sf::Sprite mySprite(textures[spriteType]);
            auto& player = players[i];
            mySprite.setPosition(SCALE * player.getX(), SCALE * player.getY());
            window.draw(mySprite);
        }
        for (size_t i = 0; i < enemies.size(); ++i)
        {
            auto spriteType = 3;
            //Tee sprite luokasta instanssi ja m‰‰rit‰ spritelle piirrett‰v‰ tekstuuri
            sf::Sprite mySprite(textures[spriteType]);
            auto& enemy = enemies[i];
            mySprite.setPosition(SCALE * enemy.getX(), SCALE * enemy.getY());
            window.draw(mySprite);
        }

        // end the current frame
        window.display();
    }


    return 0;
}