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
		winner = new Texture("assets/winner.png");

		winScreen = ecs.newEntity();
		ecs.addComponent(winScreen, Transform{ .z = 20, .xScale = 200, .yScale = 200 });
		ecs.addComponent(winScreen, SpriteRenderer{ .texture = winner, .enabled = false });
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

		if (collected >= total)
		{
			ecs.getComponent<SpriteRenderer>(winScreen).enabled = true;
		}
		TransformSystem::SetPosition(winScreen, Vector3(ecs.getComponent<Transform>(player).x, ecs.getComponent<Transform>(player).y, 20));
	}

	Entity CreatePickup(float x, float y)
	{
		Entity pickup = ecs.newEntity();
		ecs.addComponent(pickup, Transform{ .x = x, .y = y, .z = 1.5, .xScale = 35, .yScale = 25 });
		ecs.addComponent(pickup, SpriteRenderer{ .texture = defaultTexture });
		ecs.addComponent(pickup, Rigidbody{ .kinematic = true });
		ecs.addComponent(pickup, BoxCollider{ .isTrigger = true });
		ecs.addComponent(pickup, Pickup{});
		total++;

		return pickup;
	}

	Entity winScreen;
	Texture* winner;
	Texture* defaultTexture;
	int collected = 0;
	int total = 0;
};