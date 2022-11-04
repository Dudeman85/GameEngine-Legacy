# Animation and spritemap refrence

All Animation and spritemap functions are within the "engine" namespace

## Textures
Textures and images can be loaded as such:
```cpp
//Returns an sf::Texture from the image named Test Image.png
sf::Texture texture = loadTexture("Test Image.png");
//Returns an sf::Image from the image named Test Image.png
sf::Image image = loadImage("Test Image.png");
```
The image names are relative to "assetPath" which can be changed:
```cpp
assetPath = "path/"; //Relative to VS project
assetPath = "C:/temp/sprites/"; //Absolute path
```

## Spritemap
A spritemap refers to one image containing multiple sprites or different frames of animation.
There are two ways to divide a spritemap into individual sprites:

### <b>vector\<sf::Texture\> SliceSpritemap(sf::Image spritemap, int width, int height)</b>
This slices the given sf::Image spritemap into equal sized textures of given width and height
```cpp
//Slices image into 16 x 16 textures. From top-left to bottom-right
vector<sf::Teture> sprites = SliceSpritemap(image, 16, 16);
```

### <b>sf::Texture CustomSlice(sf::Image spritemap, int x1, int y1, int x2, int y2)</b>
This return a texture from a slice of the spritemap where x1 and y1 are the coordinates of the top-left
and x2 and y2 are the coordinates of the bottom-right of the slice.
```cpp
//Slices image into a 16 x 16 texture with its top-left origin at 32, 32
sf::Texture texture = CustomSlice(image, 32, 32, 48, 48);
```

## Animator
Every GameObject has an animator with every method built in.
To add animations you can automatically slice them from a spritemap, or manually give the textures.

### <b>void addAnimation(vector\<sf::Texture\> textures, vector\<int\> delays, string name)</b>
### <b>void addAnimation(vector\<sf::Texture\> textures, int delays, string name)</b>
This will take the name of the animation, a list of textures be played as frames in the animation,
as well as the amount of delay in ms between each frame.<br>
Optionally provide a single time delay to use a fixed delay between each frame
```cpp
//Adds an animation with 100ms delay after first frame, 50ms after second, and so on
//The amount of delays and frames should be the same
vector<int> delays {100, 50, 600, 4000};
gameObject.addAnimation(frames, delays, "animation1");

//Adds an animation with 100ms delay between every frame
gameObject.addAnimation(frames, 100, "animation2");
```

### <b>void autoAddAnimations(sf::Image spritemap, int width, int height, int delays, vector\<string\> names = vector\<string\>())</b>
### <b>void autoAddAnimations(sf::Image spritemap, int width, int height, vector\<int\> delays, vector\<string\> names = vector\<string\>())</b>
This will automatically slice the given spritemap and add a seperate animation for every row in it.<br>
The delay can be either given universally or per individual frame. A list of names can also be given. If not, the index of the tilemap row will be used as the name instead.
```cpp
//Adds animations with each frame sliced from spritemap with universal delays of 500ms
gameObject.autoAddAnimations(spritemap, 16, 16, 500);

//Add animations with each frame sliced from spritemap with custom delays and names
vector<int> delays {100, 50, 600, 4000};
vector<string> names {"a", "b", "c", "d"};
gameObject.autoAddAnimations(spritemap, 16, 16, delays, names);
```

To start and stop animations use
### <b>void playAnimation(string name, bool repeat = false)</b>
### <b>void stopAnimation()</b>
Play animation has an optional repeat, which will loop the animation untill stopAnimation() is called
```cpp
//Starts and repeats animation1 untill stopAnimation() is called
gameObject.playAnimation("animation1", true);
gameObject.stopAnimation();
```