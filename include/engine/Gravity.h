#pragma once
#include "ECSCore.h"
#include <box2d/box2d.h>

extern ECS ecs;

namespace engine
{
	

	struct Rigidbody
	{
		unique_ptr<b2Body> Body;
	};

	// Gravity system
	class PhysicsSystem : public System
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

		void CreateActor(b2World& world, int x, int y)
		{
			b2BodyDef BodyDef;
			BodyDef.position = b2Vec2(x / SCALE, y / SCALE);
			BodyDef.type = b2_dynamicBody;
			b2Body* Body = world.CreateBody(&BodyDef);

			b2PolygonShape Shape;
			Shape.SetAsBox((64.f / 2) / SCALE, (64.f / 2) / SCALE);
			b2FixtureDef FixtureDef;
			FixtureDef.density = 1.f;
			FixtureDef.friction = 0.7f;
			FixtureDef.shape = &Shape;
			Body->CreateFixture(&FixtureDef);


		}

		void DefineBody(Entity entity, float width, float height, float density, float friction , bool isStatic=false)
		{

		}


		void createGround(b2World& world, float x, float y)
		{
			b2BodyDef BodyDef;
			BodyDef.position = b2Vec2(x / SCALE, y / SCALE);
			BodyDef.type = b2_staticBody;
			b2Body* Body = world.CreateBody(&BodyDef);

			b2PolygonShape Shape;
			Shape.SetAsBox((64.f / 2) / SCALE, (64.f / 2) / SCALE);
			b2FixtureDef FixtureDef;
			FixtureDef.density = 0.f;
			FixtureDef.shape = &Shape;
			Body->CreateFixture(&FixtureDef);

		}
	private:
		b2Vec2 gravity;
		bool fixedRotation;
		b2World* world;
		float timeStep = 1.0f / 60.0f;
		const float SCALE = 64.f;

	};


}
