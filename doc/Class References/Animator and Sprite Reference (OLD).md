# Animator and Sprite reference

<b><u>The Animator and Render systems are members of the EngineLib class. All related components and functions are within the engine namespace</u></b>

## Images and Textures

To display anything beyond basic shapes and colors we have to load images and textures
To load an image or texture from a file you can use
```cpp
//Load spritesheet.png as an sfml image
sf::Image spritesheet = LoadImage("spritesheet.png");
//Load sprite.png as an sfml texture
sf::Texture sprite = LoadTexture("sprite.png");
```
---

## Spritesheets

A spritesheet is an image containing multiple sprites, typically frames of animation.<br>
The sprites contained can be different sized, but with this implementation it is only possible to automatically slice them if each sprite is the same size.

To get one slice from a spritesheet use
### <b>sf::Texture CustomSlice(sf::Image spritesheet, int x1, int y1, int x2, int y2)</b>
where x1 and y1 are the top left, and x2 and y2 are the bottom right coordinates of the slice.

Or, to automatically slice a spritesheet into multiple equally sized sprites use
### <b>vector\<sf::Texture\> Slicespritesheet(sf::Image spritesheet, int width, int height)</b>
where width and height are the uniform dimensions of the sprites in pixels

```cpp
//Get one slice from a spritesheet
sf::Texture defaultTexture = CustomSlice(spritesheet, 0, 0, 16, 16);
//Get all of the slices from a spritesheet
vector<sf::Texture> textures = SliceSpritesheet(spritesheet, 16, 16);
```
---

## Sprite

The sprite component contains a drawable sprite. It's origin is center.
It can be changed as such,
```cpp
//Set the entity's texture
ecs.getComponent<Sprite>(entity).texture = defaultTexture;
```
[TODO] It also has a zOverride member, which overrides the drawing order of the sprites.

---

## Rendering

The Render system is in charge of drawing sprites to an sf::RenderWindow. It requires the Sprite and Transform components.

The Render() method of the Render system will draw every sprite to the provided sf::RenderWindow, first in order of the zOverride, then by the z position. [TODO]

```cpp
//Run the renderSystem's Render method each frame
renderSystem->Render(window);
```
---

## Creating Animations

To create animations, you can either automatically slice them from a spritesheet, or manually provide each frame and timing.

### <b>vector\<Animation\> AnimationsFromspritesheet(sf::Image spritesheet, int width, int height, vector\<int\> delays)</b>
This function takes a spritesheet image, the width and height of each sprite (which must be consistent), and a vector of delay timings in ms. <br>
The animations are sliced from left to right, with one row of textures in the spritesheet becomming one animation. <br>
The timings are ordered in a 1D vector which corresponding to the spritesheet is ordered left to right top to bottom.

### <b>Animation CreateAnimation(vector\<sf::Texture\> frames, vector\<int\> delays)</b>
This function takes a vector of Textures as frames and a vector of delay timings in ms. It returns only one animation.

```cpp
//Create three animation from spritesheet with 250ms delays each
vector<Animation> animations = AnimationsFromspritesheet(spritesheet, 16, 16, vector<int>(12, 250))

//Create one animation from individual frames with varying delays
Animation animation = CreateAnimation(frames, vector<int> {100, 250, 100, 1000}) 
```
---

## Animator Component

The Animator component holds every animation available to an entity as well as all relevant information required to play animations.<br>
Although all members of the Animator component are accessible, it is not recomended to modify them, only read from them. Relevant ones are;

```cpp	
Animator animator;
//The name of the currently playing animation
animator.currentAnimation;
//The current frame of the currently playing animation
animator.animationFrame;
//True if an animation is being played
animator.playingAnimation;
//True if the animation will loop infinitely
animator.repeatAnimation;
```
---

## Animator System

As the Animator system is a member of the standard engine library, it is accessible through the EngineLib class and does not need to be manually registered.<br>
The animator system requires the Sprite abd Animator components, however the Render system also requires the Transform component, making the Animator useless without it.<br>
To add animations to entities you can use the AddAnimations() or AddAnimation() methods of the Animator system.

### <b>void AddAnimations(Entity entity, vector\<Animation\> animations, vector\<string\> names)</b>
### <b>void AddAnimation(Entity entity, Animation animation, string name)</b>

You need to provide the entity to add animations to, a vector of one or more animations, and a vector of names for each animation.
Alternatively you can just add one animation at a time.

```cpp
//Adds three animations to enemy entity
animatorSystem.AddAnimations(enemy, animations, vector<string> {"anim1", "anim2", "anim3"});

//Adds one animation to enemy entity
animatorSystem.AddAnimation(enemy, animation, "anim4");
```

To start and stop animations use
### <b>void PlayAnimation(Entity entity, string animation, bool repeat = false)</b>
### <b>void StopAnimation(Entity entity, string animation = "")</b>

PlayAnimation has an optional repeat, which will loop the animation untill StopAnimation() is called, otherwise it will end after one loop.<br>
For StopAnimation you can provide the name of the animation to stop, which only stops the animation if that animation is playing

```cpp
//Starts and repeats animation1 of enemy untill StopAnimation() is called
animatorSystem.PlayAnimation(enemy, "animation1", true);
animatorSystem.StopAnimation(enemy, "animation1");
```
---

## Usage

Below is a basic example of using the Animator system to create an animated sprite
```cpp
#include "engine/Application.h"
#include <SFML/Graphics.hpp>

//Create instances of the ECS controller and the standard engine library
ECS ecs;
engine::EngineLib lib;

int main()
{
	//Create a new entity and add the Transform, Sprite, and Animator components
	Entity player = ecs.newEntity();
	ecs.addComponent(player, engine::Transform{.x = 100, .y = 100, .xScale = 10, .yScale = 10});
	ecs.addComponent(player, engine::Sprite());
	ecs.addComponent(player, engine::Animator());

	//Load the spritesheet
	sf::Image spritesheet = engine::LoadImage("spritesheet.png");
	sf::Texture defaultTexture = engine::CustomSlice(spritesheet, 0, 0, 16, 16);

	//Set the players default texture
	ecs.getComponent<engine::Sprite>(player).texture = defaultTexture;
	
	//Add animations to player automatically sliced from the spritesheet
	lib.animationSystem->AddAnimations(player, engine::AnimationsFromSpritesheet(spritesheet, 16, 16, vector<int>(8, 250)), vector<string>{"Down", "Left", "Up", "Right"});

	//Play the "Down" animation of player on repeat
	lib.animationSystem->PlayAnimation(player, "Down", true);

	//SFML window
	sf::RenderWindow window(sf::VideoMode(800, 600), "test");

	//Main game loop
	while (window.isOpen())
	{
		//SFML window close event
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
		window.clear(sf::Color::Black);

		//Run the animationSystem's Update method each frame
		lib.animationSystem->Update();

		//Run the renderSystem's Render method each frame
		lib.renderSystem->Render(window);

		//SFML display window
		window.display();
	}
}
```