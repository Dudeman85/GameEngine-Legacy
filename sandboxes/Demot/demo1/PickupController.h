#pragma once
#include <engine/Application.h>
#include <cmath>
#include <algorithm>

extern ECS ecs;

using namespace engine;

struct Pickup
{
	bool floating = true;
};

//Pickup controller system requires sprite, transform, rigidbody, box collider and Pickup
class PickupController : public System
{
public:
	PickupController()
	{
		defaultTexture = new Texture("assets/strawberry.png");
	}

	void Update(Entity player, double programTime)
	{
		for (auto const& entity : entities)
		{
			Pickup& pickup = ecs.getComponent<Pickup>(entity);
			Transform& transform = ecs.getComponent<Transform>(entity);
			BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);

			//If floating animate it
			if (pickup.floating)
				transform.y += sin(programTime);

			if (collider.collisions.end() != find_if(collider.collisions.begin(), collider.collisions.end(), [player](const Collision& collision) { return collision.a == player; }))
			{
				collected++;
				ecs.destroyEntity(entity);
			}
		}
	}

	Entity CreatePickup(float x, float y)
	{
		Entity pickup = ecs.newEntity();
		ecs.addComponent(pickup, Transform{ .x = x, .y = y, .xScale = 20, .yScale = 20 });
		ecs.addComponent(pickup, Sprite{ .texture = defaultTexture });
		ecs.addComponent(pickup, Rigidbody{ .kinematic = true });
		ecs.addComponent(pickup, BoxCollider{ .isTrigger = true });
		ecs.addComponent(pickup, Pickup{});

		return pickup;
	}

	Texture* defaultTexture;
	int collected = 0;
};