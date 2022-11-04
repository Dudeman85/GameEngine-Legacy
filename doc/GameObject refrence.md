# GameObject class refrence

## Transform
A GameObject has an x and y position as well as width and height scale multiplier<br>
The origin is in the top left 

To manipulate the position of a GameObject:
```cpp
GameObject gameObject;

//Sets the gameObjects position
gameObject.sePosition(0, 500);

//Moves the gameObject relative to its position
gameObject.move(20, 50);

//Scales width and heigh equally
gameObject.setScale(5);

//Scales width and height seperately
gameObject.setScale(2, 6);
```

## members

You can also directly access the transform members for read and write:
```cpp
gameObject.x; //X position
gameObject.y; //Y position
gameObject.xScale; //Width multiplier
gameObject.yScale; //Height multiplier
gameObject.rotationAngle; //Rotation in degrees
```
Other members for enabling, animation and sprite are also available:
```cpp
//Disables drawing and updating of the gameObject
gameObject.enabled; 

//The default texture to be used when not playing animations
gameObject.texture; 

//True if currently playing animation, setting to false will pause the animation
gameObject.playingAnimation; 

//The current animation being played. Don't modify if not sure.
gameObject.currentAnimation; 
```

## Update
The update function sets the transform and animation for the gameObject. This should be called every frame for every gameObject.<br>
The draw function returns a sf::Sprite that can be draw to the window. This should be called whenever the gameObject needs to be displayed.
```cpp
//Updates gameObject. Call this every frame
gameObject.update();

//Draws gameObject to sf::RenderWindow window.
window.draw(gameObject.draw());
```