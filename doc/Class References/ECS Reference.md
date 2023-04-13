# Entity Component System

This reference is mean to explain how to use the ECS architecture on a surface level. For more information on how it actually works, see *ECS Report.docx*.

## ECS Basics
The ECS architectures consists of entities, components, and systems.<br>
**Entities are IDs and can be though of similarly to game objects.**<br>
**Components store data attached to entities.**<br>
**Systems operate upon the data in components.** Each system has required components it needs to operate; if an entity has every required component it will be operated upon by the system.

## Entity
Entities are IDs. They have no data or methods themselves and can be though of as gaining the attributes of the components they have.
```cpp
//Global ecs manager needs to be created before main
ECS ecs;

//Example entity
Entity player = ecs.newEntity();
```

## Component
**Components are aggregate structs or classes, meaning no user defined constructors, and no private or virtual members!** They are initialized with a designated initializer {}, just like arrays.<br>
Since components usually should not have any methods, it is good to keep them as structs.<br>
Every component also must be registered with the ECS before it can be used.

```cpp
//Example custom component	
struct Position
{
	float x, y, z;
};

//Register custom component
ecs.registerComponent<BoxCollider>();

//Add the component to an entity
ecs.addComponent(player, Position{ .x = 10, .y = 25, .z = 0});
```

## Engine Library
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
- PhysicsSystem: Requires 
