# Animator refrence

The animator system and all related components and functions are within the engine namespace

## Creating Animations
To create animations, you can either automatically slice them from a spritemap, or manually provide each frame and timing. <br>
Automatic Method:

### <b>vector\<Animation\> AnimationsFromSpritemap(sf::Image spritemap, int width, int height, vector\<int\> delays)</b>
<br>
This function takes a spritemap image, the width and height of each sprite (which must be consistent), and a vector of delay timings in ms. <br>
The animations are sliced from left to right, with one row in the spritemap being one animation. <br>
The timings are ordered in a 1D vector which corresponding to the spritemap is ordered left to right top to bottom. <br><br>

### <b>Animation CreateAnimation(vector\<sf::Texture\> frames, vector\<int\> delays)</b>
<br>
This function takes a vector of Textures as frames and a vector of delay timings in ms. It returns only one animation.

```cpp
//Create three animation from spritemap
vector<Animation> animations = AnimationsFromSpritemap(spritemap, 16, 16, vector<int> {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3})

//Create one animation from individual frames
Animation animation = CreateAnimation(frames, vector<int> {1, 1, 1, 1}) 
```

## Animator
To add animations to entities you can use the AddAnimations() or AddAnimation() method of the Animator system.

### <b>void AddAnimations(Entity entity, vector\<Animation\> animations, vector\<string\> names)</b>
### <b>void AddAnimation(Entity entity, Animation animation, string name)</b>
<br>
You need to provide the entity to add animations to, a vector of one or more animations, and a vector of names for each animation. <br>
Alternatively you can just add one animation at a time. <br>

```cpp
//Adds three animations to enemy entity
animatorSystem.AddAnimations(enemy, animations, vector<string> {"anim1", "anim2", "anim3"});

//Adds one animation to enemy entity
animatorSystem.AddAnimation(enemy, animation, "anim4");
```

To start and stop animations use <br>
### <b>void PlayAnimation(Entity entity, string animation, bool repeat = false)</b>
### <b>void StopAnimation(Entity entity, string animation = "")</b>
<br>
PlayAnimation has an optional repeat, which will loop the animation untill StopAnimation() is called, otherwise it will end after one loop.<br>
For StopAnimation you can provide the name of the animation to stop, which only stops the animation if that animation is playing

```cpp
//Starts and repeats animation1 of enemy untill StopAnimation() is called
animatorSystem.PlayAnimation(enemy, "animation1", true);
animatorSystem.StopAnimation(enemy);
```