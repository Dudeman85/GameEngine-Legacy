#pragma once
#include <engine/Application.h>
#include <cmath>
#include <algorithm>

extern ECS ecs;

using namespace engine;

struct Pickup
{
	bool floating = false;
	int sound = 0;
};

//Pickup controller system requires sprite, transform, rigidbody, box collider and Pickup
class PickupController : public System
{
public:
	PickupController()
	{
		defaultTexture = new Texture("assets/ammo box.png");
	}

	void Update(Entity player)
	{
		for (auto const& entity : entities)
		{
			Pickup& pickup = ecs.getComponent<Pickup>(entity);
			Transform& transform = ecs.getComponent<Transform>(entity);
			BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);

			//If collided with the player start the collection animation
			if (collider.collisions.end() != find_if(collider.collisions.begin(), collider.collisions.end(), [player](const Collision& collision) { return collision.a == player; }))
			{
				collected++;
				ecs.destroyEntity(entity);
				break;
			}
		}
	}

	Entity CreatePickup(float x, float y)
	{
		Entity pickup = ecs.newEntity();
		ecs.addComponent(pickup, Transform{ .x = x, .y = y, .z = 1.5, .xScale = 35, .yScale = 25 });
		ecs.addComponent(pickup, Sprite{ .texture = defaultTexture });
		ecs.addComponent(pickup, Rigidbody{ .kinematic = true });
		ecs.addComponent(pickup, BoxCollider{ .isTrigger = true });
		ecs.addComponent(pickup, Pickup{});

		return pickup;
	}

	Texture* defaultTexture;
	int collected = 0;
};