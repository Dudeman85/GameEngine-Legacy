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

