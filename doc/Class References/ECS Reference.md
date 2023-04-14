# Entity Component System

This reference is mean to explain how to use the ECS architecture on a surface level. For more information on how it actually works, see *ECS Report.docx*.

## ECS Basics
The ECS architectures consists of entities, components, and systems.<br>
**Entities are IDs and can be though of similarly to game objects.**<br>
**Components store data attached to entities.**<br>
**Systems operate upon the data in components.** Each system has required components it needs to operate; if an entity has every required component it will be operated upon by the system.

To use the ECS implementation you need to create a global instance of ECS before your main function and declare it as external in every file you use ECS in.
```cpp
//The ECS implementation
#include <engine/ECSCore.h>	

//Global ECS manager needs to be created once before main
ECS ecs;

//In every file using ECS declare it as external
extern ECS ecs;
```

## Entity
Entities are IDs. They have no data or methods themselves and can be though of as gaining the attributes of the components they have.
```cpp
//Example entity
//This is just an id with no actual data
Entity player = ecs.newEntity();
```

## Component
**Components are aggregate structs or classes, meaning no user defined constructors, and no private or virtual members!** They are initialized with a designated initializer {}, just like arrays.<br>
Since components usually should not have any methods, it is good to keep them as structs.<br>
Every component also must be registered with the ECS manager before it can be used.

```cpp
//Example custom component	
struct Position
{
	float x, y, z;
};

//Register custom component
ecs.registerComponent<Position>();

//Add the component to an entity
ecs.addComponent(player, Position{ .x = 10.0f, .y = 25.25f, .z = 0.0f});

//Get the Position component of player
Position& position = ecs.getComponent<Position>(player);

//You can read/write to the variables of a component directly
position.x = 5.1f;
```

## System
Systems are esentially collections functions that operate upon data in components. Each system has a list of required components it needs to operate, as well as a list of entities with those required components.<br>
For example, a render system could require Sprite and Transform components. It would then automatically operate upon every entity with at least those components.<br>

Every system must be a class that inherits from the System class, this class will contain a list of all entities with the required components.<br>
Once you create a system, you need to register it with the ECS manager and set its signature of required components.
```cpp
//Example Gravity System
//Requires Position component
class GravitySystem : public System
{
public:
	//The Update function of the Gravity system should be called every frame
	void Update(float deltaTime)
	{
		//Every systems Update method should operate upon the entire entities list
		for (auto const& entity : entities)
		{
			//Get the Position component of the entity
			//There is no need to make sure it has one since the ECS manager takes care of that
			Position& position = ecs.getComponent<Position>(entity);

			//Operate upon the data in the entity's Position component
			position.x += gravity.x;
			position.y += gravity.y;
			position.z += gravity.z;
		}
	}

	engine::Vector3 gravity = Vector3(0, -9.81, 0);
}

//Register the custom GravitySystem
shared_ptr<GravitySystem> gravitySystem = ecs.registerSystem<GravitySystem>();

//Create a new signature and set it to require the Position component
Signature gravitySystemSignature;
gravitySystemSignature.set(ecs.getComponentId<Position>());

//Assign the signature to the GravitySystem
ecs.setSystemSignature<GravitySystem>(gravitySystemSignature);

//Now you can use the gravity system
//It will automatically operate upon every entity with the Position component
gravitySystem->Update();
```

## Engine Library

### ***All engine related classes and functions are within the engine namespace!***

The engine comes with multiple premade components and systems, these do not need to be manually registered. Their functionality should be self-explanatory, but each is explained more thoroughly in its own reference.<br>

Components:
- Sprite
- Animator
- Transform
- Rigidbody
- BoxCollider

Systems:
- TransformSystem: Requires Transform
- RenderSystem: Requires Transform and Sprite
- AnimationSystem: Requires Sprite and Animator
- PhysicsSystem: Requires Rigidbody, Transform, and BoxCollider

To use these create a window and an instance of EngineLib. The window must be created first, otherwise OpenGL will fail to load.
```cpp
//Includes engine library and all default ECS functionality
#include <engine/Application.h>

//Everything is in the engine namespace
using namespace engine;

//Create the window and OpenGL context before creating EngineLib
GLFWwindow* window = CreateWindow(800, 600, "Window");

//Initialize the default engine library
EngineLib engine;

//You can access each default engine system
engine.physicsSystem->gravity = Vector2(0, -400);

//This will update all default engine systems and calculate deltaTime
double deltaTime = engine.Update(&cam);
```