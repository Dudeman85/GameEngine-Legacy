# Sprite Functionality

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

You can also slice a sub-image from an image
```cpp
//Get a slice of testImage from (x1, y1) to (x2, y2). Both are inclusive
Image slice = testImage.Slice(x1, y1, x2, y2);
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
You can write your own fragment and vertex shaders an compile them to a shader with the Shader class.
```cpp
//Create a new shader by loading the fragment and vertex shaders from file
//Leaving either of these empty will use the default shader
Shader shader = Shader("vertexShader.glsl", "fragmentShader.glsl");
```

## Sprite Component

The sprite component only contains a pointer to a texture and optionally a pointer to a shader.<br>
Every sprite could be drawn with a different shader if desired. If no shader is specified the default is used.
```cpp
//Create a new entity
Entity sprite = ecs.newEntity();
//Add the sprite component with a texture and custom shader
ecs.addComponent(sprite, Sprite{&texture, &shader});
```