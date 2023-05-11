# Physics Reference

### ***The default Physics system is a member of the EngineLib class! All related components and functions are within the engine namespace!***

---
## Transform
The transform component stores the position, rotation, and scale of an entity. It currently stores these as individual floats for the x, y, and z components.

Currently the transform system's update method has no functionality, and the other functionality is also incomplete.

These static methods are implemented:
```cpp
//Add the Transform component
Transform& playerTransform = ecs.addComponent(player, Transform{ .x = 0, .y = 25, .xScale = 20, .yScale = 20 });

//Current Transform system static methods using either Vector3 or 3 floats
TransformSystem::Translate(player, 0, 10, 0);
TransformSystem::Translate(player, Vector3(0, 10, 0));
TransformSystem::SetPosition(player, 20, 500, 0);
TransformSystem::SetPosition(player, Vector3(20, 500, 0));

//You can read and write to members directly
float x = playerTransform.x;
float yScale = playerTransform.yScale;
playerTransform.zRotation = 45;
```

---
## Rigidbody

The Rigidbody component contain all the physics material properties of the entity, including:
- velocity: The current velocity of the entity
- gravityScale: Multiplier of how much this entity is affected by gravity
- drag: Atmospheric drag coefficient for this entity
- friction: Friction coefficient, total friction is calculated as the averae of the two rubbing materials
- elasticity: Esentially bounciness
- kinematic: If true this entity will not be affected by physics calculations, except for collision

```cpp
//Add the Rigidbody component
Rigidbody& playerRigidbody = ecs.addComponent(player, Rigidbody{ .drag = 0.1, .friction = 0.2, .elasticity = 0.125 });

//You can read and write to members directly
playerRigidbody.velocity = Vector2(0, 900);
float playerFriction = playerRigidbody.friction;
```

---
## BoxCollider

Collision are automatically checked only when an entity is moved using the PhysicsSystem::Move() method.<br>
The box collider component determines the axis-aligned bounding box of the entity.
Normally it is the same size as the sprite, but it can be scaled and translated, **this is not done automatically when rotating an entity**:
```cpp
//Make the box collider taller and move it to the left
ecs.addComponent(player, BoxCollider{ .scale = Vector2(1, 1.5), .offset = Vector2(-10, 0)});
```

The *isTrigger* member is used to determine if this collider should stop other colliders from intersecting it. If it is true the collider will not be "solid", but will still log the collision.

The *sidesCollided* member is an array that stores the collision status of each side of the collider.<br>
Directions are defined as 0 = up, 1 = right, 2 = down, 3 = right. There is also a Direction enum so these numbers don't need to be memorized.

The *collisions* member is a vector that stores every collision that happened involving this entity during the previous collision check.

### Collision Struct

The Collision struct holds all relevant information about a single collision check:
- type: The type of collision this was. Possible types are in the enum Collision::Type { entity, tilemap, trigger, miss }
    - entity: The collision check was between an entity and and entity
    - tilemap: The collision check was between an entity and a tilemap collider
    - trigger: One of the colliders was defined as a trigger; Physics System does not act upon this, but it is still logged
    - miss: The two colliders did not intersect; this is mostly used internally by the Physics System
- a: The entity which instigated the collision
- b: The entity which was subject to the collision, in the case of a tilemap collision this is undefined
- tileID: The ID of the tilmap collider tile involved in the collision, in the case of an entity collision this is undefined
- intersects: An array containing how much collider b has overlapped collider a on each side
- side: The side of a which collided; the minimum non-zero intersect

---
## PhysicsSystem

The physics system is in charge of applying forces on entities, updating the entity's position based on its velocity, and checking for collision. It requires the Transform, Rigidbody, and BoxCollider components.

Moving entities can be done directly or with impulse. The Move() method is used to move an entity while checking collision, as opposed to Translate() which does not check collision.
```cpp
//Move the entity while checking collision, most physics calculations should be done with respect to deltaTime
engine.physicsSystem->Move(player, Vector2(0, 500) * engine.deltaTime);
//Add an impulse to entity
PhysicsSystem::Impulse(player, Vector2(100, 100));
```

If you need to manually check for collision:<br>
(this is rarely necessary since collision is checked whenever any entity is moved)
```cpp
//Performs AABB collision detection between player and every other entity with a collider as well as the tilemap collider if it exists
vector<Collision> playerCollisions = engine.physicsSystem->DetectCollision(player);

//Returns a Collision containing data about the axis-aligned bounding box intersects of player and enemy
//If there was no intersect it returns a Collision of type miss
Collision playerEnemyCollision = PhysicsSystem::AABBIntersect(player, enemy);

//Check intersect between tilemap collision layer and entity a
vector<Collision> tilmapCollisions = engine.physicsSystem->TilemapIntersect(a);
```

Other methods:
```cpp
//Gets the min and max bounds of the entity's collider
array<float, 4> playerBounds = PhysicsSystem::GetBounds(player);

//Set the gravity
engine.physicsSystem->gravity = Vector2(0, -981);
```

As with most ECS systems, PhysicsSystem functions that operate upon only one entity don't usually need to be members of the system class. However here they are static members for the sake of organization.
```cpp
//These are equivalent
PhysicsSystem::Impulse(player, Vector2(100, 100));
engine.physicsSystem->Impulse(player, Vector2(100, 100));
```