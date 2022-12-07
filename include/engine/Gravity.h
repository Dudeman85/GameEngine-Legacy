#pragma once
#include "ECSCore.h"
#include <box2d/box2d.h>

extern ECS ecs;

namespace engine
{
	b2Vec2 gravity(0.0f, 0.0f);
	bool fixedRotation;
	b2World* world;
	float timeStep = 1.0f / 60.0f;
	static const float SCALE = 64.f;

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

		void CreateBox(b2World& World, int X, int Y)
		{
			b2BodyDef BodyDef;
			BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
			BodyDef.type = b2_dynamicBody;
			b2Body* Body = World.CreateBody(&BodyDef);

		}

		void createGround(b2World& World, float X, float Y)
		{
			b2BodyDef BodyDef;
			BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
			BodyDef.type = b2_staticBody;
			b2Body* Body = World.CreateBody(&BodyDef);

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
