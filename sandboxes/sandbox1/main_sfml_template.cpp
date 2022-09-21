#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string>

//assetit eri kansiosta riippuen ollaanko debug vai relese k‰‰nnˆksess‰.
#if defined(_DEBUG)
std::string ASSET_PATH = "../assets/";
#else
std::string ASSET_PATH = "assets/";
#endif

sf::Texture loadTexture(const std::string& fileName) {
    sf::Texture texture;
    if (!texture.loadFromFile(ASSET_PATH + fileName)) {
        // error...
        printf("Error loading image!\n");
    }
    return texture;
}

class GameObject {
public:
    GameObject(float px, float py) 
        : positionX(px), positionY(py) {

    }

    float getX() const {
        return positionX;
    }
    float getY() const {
        return positionY;
    }

    void move(float deltaX, float deltaY) {
        positionX += deltaX;
        positionY += deltaY;
    }
private:
    float positionX;
    float positionY;
};

bool sphereSphereCollisionCheck(const GameObject& o1, const GameObject& o2) {
    float deltaX = o2.getX() - o1.getX();
    float deltaY = o2.getY() - o1.getY();
    float c = abs(deltaX * deltaX + deltaY * deltaY);
    float r1PlusR2 = 1.0f; // oletetaan, ett‰ peliobjektit ovat ykkˆsen kokoisia, ts r1 = 0,5, r2 = 0,5 -> r1+r2 == 1
    return c < r1PlusR2;

}

int main() 
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    std::vector<sf::Texture> textures = {
        
        loadTexture("image1.png"),
        loadTexture("image2.png"),
        loadTexture("player.png"),
        loadTexture("enemy.png")// indeksi 2
    };

  
   
    sf::Clock frameTimer;

    // 8-bit value to describe, what kind of game object os oin location x,y.
    // mapp‰‰ x,y koordinaatit 8 bit unsigned integeriksi.
    const float  SCALE = 64.0f;
    std::vector< std::vector<uint8_t> > map = {
        {1, 1, 1, 1},
        {1, 0, 0, 1},
        {1, 0, 0, 1},
        {1, 1, 1, 1}
        
    };

    std::vector<GameObject> players = {
        GameObject(0,0)
    };

    std::vector<GameObject> enemies = {
        GameObject(4,2)
    };



    sf::Sprite mySprite(textures[0]);


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
        float deltaTime= frameTimer.getElapsedTime() .asSeconds();
        frameTimer.restart();
        // clear the window with black color
        window.clear(sf::Color::Black);

        // 1. read input
        float deltaX = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            // left key is pressed: move our character
            deltaX -= 2.0f * deltaTime;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // left key is pressed: move our character
            deltaX += 2.0f * deltaTime;
        }

        float deltaY = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            // down key is pressed: move our character
            deltaY -= 2.0f * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
         
        {
           // up key is pressed: move our character
            deltaY += 2.0f * deltaTime;
        }
        
        // 2. Move game objects according to input (update)

        players[0].move(deltaX, deltaY);
        


        // Check collisions
        for (size_t playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
            for (size_t enemyIndex = 0; enemyIndex < enemies.size(); ++enemyIndex) {
                if (sphereSphereCollisionCheck(players[playerIndex], enemies[enemyIndex])) {
                    printf("Player: %d collides with enemy: %d\n", (int)playerIndex, (int)enemyIndex);
                }
            }
        }

        // Read input
        // mySprite.setScale(0.1f, 0.1f);
        // mySprite.setRotation(45.0f); 
        // mySprite.rotate(360.0f * deltaTime);
        // mySprite.setPosition(200.0f, 100.0f);

        // 3. "render"
        // Piirr‰ sprite ruudulle
        // window.draw(mySprite);
        // piirr‰ ensin map ruudulle


        for (size_t y = 0; y < map.size(); ++y) {
            for (size_t x = 0; x < map[y].size(); ++x) {
                auto spriteType = map[y][x];
                sf::Sprite mySprite(textures[spriteType]);
                mySprite.setPosition(SCALE*x, SCALE*y);
                window.draw(mySprite);
            }
        }

        // piirr‰ sitten playerit ruudulle
        for (size_t i = 0; i < players.size(); ++i) {
            auto spriteType = 2; // Player sprite indeksi 2
            sf::Sprite mySprite(textures[spriteType]);
            auto& player = players[i];
            mySprite.setPosition(SCALE * player.getX(), SCALE * player.getY());
            window.draw(mySprite);
        }
            // piirr‰ sitten enemyt ruudulle
            for (size_t i = 0; i < enemies.size(); ++i) {
                auto spriteType = 3; // Enemy sprite indeksi 3
                sf::Sprite mySprite(textures[spriteType]);
                auto& enemy = enemies[i];
                mySprite.setPosition(SCALE * enemy.getX(), SCALE * enemy.getY());
                window.draw(mySprite);
        }

        
        // 4: end the current frame (swap buffers)
        window.display();

      
    }

    return 0;
}