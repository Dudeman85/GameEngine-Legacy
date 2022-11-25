#pragma once
#include "ECSCore.h"
#include <box2d/box2d.h>

extern ECS ecs;

namespace engine
{
	b2Vec2 gravity(0.0f, 0.0f);
	struct Gravity
	{
		float x, y;

	};
	b2World* world;
	float timeStep = 1.0f / 60.0f;


	// Gravity system
	class GravitySystem : public System
	{
	public:
		void Update()
		{
			for (auto const& entity : entities)
			{

			}
		}
		void Init(b2Vec2 gravity = b2Vec2_zero)
		{
			gravity.Set(gravity.x, gravity.y);
			world->SetGravity(gravity);
		}


		void setGravity(Entity entity, float x, float y)
		{
			Gravity& gravity = ecs.getComponent<Gravity>(entity);
			gravity.x = x;
			gravity.y = y;
		
		}

	};


}
//Box2D
//b2Vec2 gravity(0.0f, 0.0f);
//float timeStep = 1.0f / 60.0f;
//int32 velocityIterations = 6;
//int32 positionIterations = 2;
//
//
