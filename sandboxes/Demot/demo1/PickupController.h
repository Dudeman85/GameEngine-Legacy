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

//Pickup controller system requires sprite, transform, rigidbody, box collider, Animator and Pickup
class PickupController : public System
{
public:
	PickupController()
	{
		defaultTexture = new Texture("assets/strawberry.png");
		animations = AnimationsFromSpritesheet("assets/Strawberry Animation.png", 7, 2, vector<int>(7*2, 100));
	}

	void Update(Entity player, double programTime)
	{
		for (auto const& entity : entities)
		{
			Pickup& pickup = ecs.getComponent<Pickup>(entity);
			Transform& transform = ecs.getComponent<Transform>(entity);
			BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);
			Animator& animator = ecs.getComponent<Animator>(entity);

			//If floating animate it
			if (pickup.floating)
				transform.y += sin(programTime * 3) * 0.5;

			//If collided with the player start the collection animation
			if (collider.collisions.end() != find_if(collider.collisions.begin(), collider.collisions.end(), [player](const Collision& collision) { return collision.a == player; }))
			{
				AnimationSystem::PlayAnimation(entity, "collect");
				//PUT SOUND HERE
			}

			//When the collection animation is over
			if (!animator.playingAnimation)
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
		ecs.addComponent(pickup, Transform{ .x = x, .y = y, .z = 5, .xScale = 30, .yScale = 25 });
		ecs.addComponent(pickup, Sprite{ .texture = defaultTexture });
		ecs.addComponent(pickup, Rigidbody{ .kinematic = true });
		ecs.addComponent(pickup, BoxCollider{ .isTrigger = true });
		ecs.addComponent(pickup, Pickup{});
		ecs.addComponent(pickup, Animator{});
		AnimationSystem::AddAnimations(pickup, animations, vector<string>{"default", "collect"});
		AnimationSystem::PlayAnimation(pickup, "default", true);

		return pickup;
	}

	vector<Animation> animations;
	Texture* defaultTexture;
	int collected = 0;
};