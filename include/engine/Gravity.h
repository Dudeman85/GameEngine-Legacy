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

		void CreateActor(b2World& World, int X, int Y)
		{
			b2BodyDef BodyDef;
			BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
			BodyDef.type = b2_dynamicBody;
			b2Body* Body = World.CreateBody(&BodyDef);

			b2PolygonShape Shape;
			Shape.SetAsBox((64.f / 2) / SCALE, (64.f / 2) / SCALE);
			b2FixtureDef FixtureDef;
			FixtureDef.density = 1.f;
			FixtureDef.friction = 0.7f;
			FixtureDef.shape = &Shape;
			Body->CreateFixture(&FixtureDef);

		}

		void createGround(b2World& World, float X, float Y)
		{
			b2BodyDef BodyDef;
			BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
			BodyDef.type = b2_staticBody;
			b2Body* Body = World.CreateBody(&BodyDef);

			b2PolygonShape Shape;
			Shape.SetAsBox((64.f / 2) / SCALE, (64.f / 2) / SCALE);
			b2FixtureDef FixtureDef;
			FixtureDef.density = 0.f;
			FixtureDef.shape = &Shape;
			Body->CreateFixture(&FixtureDef);

		}

	};


}
