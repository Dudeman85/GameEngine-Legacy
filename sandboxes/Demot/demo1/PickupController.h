#pragma once
#include <engine/Application.h>
#include <cmath>
#include <algorithm>

extern ECS ecs;

using namespace engine;

struct Pickup
{
	bool floating = true;
	int sound = 0;
};

struct  Board
{
	int vicCondition = 0;
	glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
};

//Pickup controller system requires sprite, transform, rigidbody, box collider, Animator and Pickup
class PickupController : public System
{
public:
	PickupController()
	{
		defaultTexture = new Texture("assets/strawberry.png");
		animations = AnimationsFromSpritesheet("assets/Strawberry Animation.png", 7, 2, vector<int>(7*2, 100));
		scoreAnims = AnimationsFromSpritesheet("assets/");
	}

	void Update(Entity player, double programTime)
	{
		for (auto const& entity : entities)
		{
			Pickup& pickup = ecs.getComponent<Pickup>(entity);
			Board& board = ecs.getComponent<Board>(entity);
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
				pickup.sound = 1;
			}

			//When the collection animation is over
			if (!animator.playingAnimation)
			{
				collected++;
				AnimationSystem::PlayAnimation(en, to_string(collected));
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

	Entity CreateScoreboard(float x, float y)
	{
		Entity board = ecs.newEntity();
		ecs.addComponent(board, Transform{ .x = x, .y = y });
		ecs.addComponent(board, Animator{});
		ecs.addComponent(board, Board{});
		AnimationSystem::AddAnimations(board, scoreAnims, vector<string>{"0", "1", "2", "3", "4",
			"5", "6", "7", "8", "9", });
		AnimationSystem::PlayAnimation(board, "0", true);
	}

	vector<Animation> scoreAnims;
	vector<Animation> animations;
	Texture* defaultTexture;
	int collected = 0;
};