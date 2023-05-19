#pragma once
#include <engine/Application.h>

extern ECS ecs;

using namespace engine;

struct Turret
{
	int health;
};

//Pickup controller system requires sprite, transform, rigidbody, box collider, animator and Turret
class TurretController : public System
{
public:
	TurretController()
	{
		//defaultAnimations = AnimationsFromSpritesheet();
	}

	void Update()
	{
		for (auto const& entity : entities)
		{


		}
	}

	Entity CreateTurret(float x, float y)
	{
		Entity turret = ecs.newEntity();
		ecs.addComponent(turret, Transform{ .x = x, .y = y, .xScale = 20, .yScale = 20 });
		ecs.addComponent(turret, Sprite{});
		ecs.addComponent(turret, Rigidbody{});
		ecs.addComponent(turret, BoxCollider{});
		ecs.addComponent(turret, Animator{});
		ecs.addComponent(turret, Turret{});
		AnimationSystem::AddAnimation(turret, animation, "default");

		return turret;
	}

	Animation animation;
	Animation projectileAnim;
};