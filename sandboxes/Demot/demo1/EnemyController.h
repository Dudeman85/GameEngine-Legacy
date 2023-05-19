#pragma once
#include <engine/Application.h>

extern ECS ecs;

using namespace engine;

struct Enemy
{
	int health;
};

//Pickup controller system requires sprite, transform, rigidbody, box collider, animator and Enemy
class EnemyController : public System
{
public:
	EnemyController()
	{
		//defaultAnimations = AnimationsFromSpritesheet();
	}

	void Update()
	{
		for (auto const& entity : entities)
		{


		}
	}

	Entity CreateEnemy(float x, float y)
	{
		Entity enemy = ecs.newEntity();
		ecs.addComponent(enemy, Transform{ .x = x, .y = y, .xScale = 20, .yScale = 20 });
		ecs.addComponent(enemy, Sprite{});
		ecs.addComponent(enemy, Rigidbody{});
		ecs.addComponent(enemy, BoxCollider{});
		ecs.addComponent(enemy, Enemy{});

		return enemy;
	}

	vector<Animation> defaultAnimations;
	Texture* defaultTexture;
	int collected = 0;
};