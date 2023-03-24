# Sprite Reference

<b><u>The default Animation and Render systems are members of the EngineLib class. All related components and functions are within the engine namespace</u></b>

## Images

An image is just a 2D vector of Pixels containing red, green, blue, and alpha values.<br>
To load an image from a file you can use the Image class constructor
```cpp
//Load a new image
Image testImage("image.png");
```

You can index the image to get a specific pixel. An images origin is its top left corner.
```cpp
//Get the pxel at position (x, y)
Pixel pixel = testImage[x][y];

//Pixel members
pixel.r
pixel.g
pixel.b
pixel.a
```

You can also slice a sub-image from an image:
```cpp
//Get a slice of testImage from (x1, y1) to (x2, y2). Both are inclusive
Image slice = testImage.Slice(x1, y1, x2, y2);
```

If you have a spritesheet that you wish to slice into multiple textures:
```cpp
//Create multiple textures from a spritesheet, going from top-left to bottom-right.
//The parameters are: path to image, how many sprites wide, and how many sprites tall.
std::vector<Texture*> textures = SliceSpritesheet("spritesheet.png", 4, 6);
```

The image class has members for width and height. These should only be read from and not modified unless absolutely sure.
```cpp
//Image width and height members
testImage.width;
testImage.height;
```

## Textures

Textures are OpenGL textures they are what get rendered when drawing sprites.<br>
You can load a texture straight from a file, or from an Image. You can also specify the filtering type.
```cpp
//Load a texture from file
Texture texture("gradient.png", GL_NEAREST);
//Create a texture from an image
Texture texture2(testImage, GL_LINEAR);

//You can also change the filtering mode
texture.SetScalingfilter(GL_LINEAR);
```

It is important to note that the origin of OpenGL textures is the bottom left, so when creating a texture from an image it is automatically flipped.<br>
There is no way to create an image out of a texture.

## Shaders

Shaders determine how a sprite or other object is rendered. Shaders are written in glsl. <br>
You can write your own fragment and vertex shaders and compile them to a shader with the Shader class.
```cpp
//Create a new shader by loading the fragment and vertex shaders from file
//Leaving either of these empty will use the default shader for that type
Shader shader = Shader("vertexShader.glsl", "fragmentShader.glsl");
```

## Sprite Component

The sprite component only contains a pointer to a texture and optionally a pointer to a shader.<br>
Every sprite can be drawn with a different shader if desired. If no shader is specified the default is used.<br>
All entities with the sprite component will be drawn by the RenderSystem.
```cpp
//Create a new entity
Entity sprite = ecs.newEntity();
//Add the sprite component with a texture and custom shader
ecs.addComponent(sprite, Sprite{&texture, &shader});
```

## Animations

An animation is simply a list of textures to be used as frames and a list of ints to be used as delays in milliseconds.<br>
You can manually create it from textures, or automatically create it from a spritesheet.

```cpp
//Create multiple textures from a spritesheet.
std::vector<Texture*> slicedTextures = SliceSpritesheet("spritesheet.png", 4, 6);
//Copy the first row of the spritesheet to a new vector
std::vector<Texture*> textures(4);
std::copy(slicedTextures.begin(), slicedTetures.begin() + 4, textures.begin());
//Create a vector of delays with each delay being 250ms
std::vector<int> delays(4, 250);

//Create the animation
Animation anim = Animation(textures, delays);

//Add the Animator component to the sprite entity
ecs.addComponent(sprite, Animator{});

//Add the anim animation with the name "Animation 1" to the sprite entity
engineLib.animationSystem->AddAnimation(sprite, anim, "Animation 1");


//Create animations from a spritesheet where one animation contains all the textures from one row of the spritesheet
//The parameters are: the path to the image, how many sprites wide, how many sprites tall, and a vector of delays for each frame
std::vector<Animation> anims = AnimationsFromSpritesheet("spritesheet.png", 4, 6, vector<int>(24, 250));

//Add the first animation, aka the first row, created from the spritesheet
//This has essentially the same results as the above example
engineLib.animationSystem->AddAnimation(sprite, anims[0], "Animation 2");
```

Animation control:
```cpp
//Start playing an animation named "Animation 1", without looping
engineLib.animationSystem->PlayAnimation(sprite, "Animation 1", false);
//Stop playing the currently playing animation
engineLib.animationSystem->StopAnimation(sprite);

//Get the sprite entity's Animator component
Animator animator = ecs.getComponent<Animator>(sprite);
//The animator component has members which can be used to get its current state.
//These should not be directly modified unless you know what you're doing. 
//For example if you wanted to temporarily pause the animation you could change playingAnimation to false.
animator.currentAnimation;
animator.animationFrame;
animator.repeatAnimation;
animator.playingAnimation;
animator.animationTimer;
```