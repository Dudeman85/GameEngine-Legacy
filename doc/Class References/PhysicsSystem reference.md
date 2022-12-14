# PhysicsSystem

<b><u> PhysicsSystem is a member of the EngineLib class. Components and functions use engine namespace</u></b>

## Physics

This system uses box2d-physics engine as basis. It includes collisions, gravity, mass and friction. It uses MKS units, therefore measurements are in meters, kilograms and seconds.
However, body measurements are scaled to 64x64 pixels.

```cpp
//defines the size of created body
b2PolygonShape shape;
shape.SetAsBox((width / 2) / SCALE, (height / 2) / SCALE);

const float SCALE = 64.f;
```
---

## Setting up Gravity

To set up gravity in your project, use
### lib.physicsSystem->Init(0.0f, 9.8f);

First value set gravity for x-axis, and the second value for y-axis.
For real world gravity, use 9,8 for y-axis.
While there is no gravitational pull on horizontal plain in real world, changing X-axis value can be used to simulate strong wind.

---

## Defining bodies

To set up rigid body, you need to use
### lib.physicsSystem->DefineBody(player, 16.f, 16.f);

First is the name of your entity, second and third values are size of the box in pixels(x, y).
Gravity and forces effect this entity.

If you want to create static body, that does not move according to gravity, use
### lib.physicsSystem->DefineBody(ground, 64, 16, true);

First 3 values are the same as in rigid body, last value makes it static.

---

## Velocity settings

Velocity is defined in engine like this:

```cpp
switch (moveState)
			{
			case 1:  velocity.x = -vx; break; //Left
			case 2:  velocity.x = 0; break; //stop
			case 3: velocity.x = vx; break; //right
			case 4: velocity.y = -vy; break; //Jump
			}
```

To add movement to your character, you need to define movement speed yourself, preferably in seperate header file.

```cpp
//player component
struct Player 
{
	float moveSpeed = 15.f;
	float jumpSpeed = 30.f;
	int moveState;
};
```
---
To bind movement to keyboard input,(with sfml) use following:

```cpp
void Update()
	{
		for (auto const& entity : entities)
		{
			Player& player = ecs.getComponent<Player>(entity);
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				lib->physicsSystem->Velocity(entity, player.moveSpeed,0,1);
            }
        }
    }
```
This binds left movement to 'A' key. In velocity part, it binds movement to player entity,
adds movement speed in X-axis (defined earlier), sets Y-axis movement to 0 and binds it to moveState case 1.

For jumping you need to make key press register only once, otherwise your character will fly.
To do this, use in for loop:
```cpp
if (prevJumped == false && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				lib->physicsSystem->Velocity(entity, 0, player.jumpSpeed, 4);
			}
```
and inside Update function, below for -loop
```cpp
prevJumped = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
```

You also need to create boolean for prevJumped outside Update function
```cpp
bool prevJumped = false;
```
