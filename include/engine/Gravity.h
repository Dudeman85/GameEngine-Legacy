#pragma once
#include "ECSCore.h"
#include <box2d/box2d.h>

extern ECS ecs;

namespace engine
{
	//Rigisbody component
	struct Rigidbody
	{
		b2Body* body;
	};

	//Physics system
	class PhysicsSystem : public System
	{
	public:

		void Update()
		{
			//defines the timestep of the physics engine. Velocity iteration is 8 and position iteration 3. These are suggested by Box2d, smaller values decrease accuracy but increase performance
			world->Step(timeStep, 8, 3);

			for (auto const& entity : entities)
			{
				Rigidbody& rigidbody = ecs.getComponent<Rigidbody>(entity);
				Transform& transform = ecs.getComponent<Transform>(entity);
				transform.x = rigidbody.body->GetPosition().x;
				transform.y = rigidbody.body->GetPosition().y;
			}
		}

		//Initializes the Box2D world with desired gravity. MUST BE RUN BEFORE USING PHYSICS SYSTEM!
		void Init(float x, float y)
		{
			gravity.Set(x, y);
			world = new b2World(gravity);
			world->SetGravity(gravity);
		}

		//Creates new box2d rigidbody and gives size, density and friction to entity and defines whether or not it is static
		void DefineBody(Entity entity, float width, float height, bool isStatic = false, float density = 1.f, float friction = 0.7f)
		{
			Rigidbody& rigidbody = ecs.getComponent<Rigidbody>(entity);
			Transform& transform = ecs.getComponent<Transform>(entity);

			//defines a new body, it's position and whether or not it is static.
			b2BodyDef bodyDef;
			bodyDef.position = b2Vec2(transform.x, transform.y);
			bodyDef.type = isStatic ? b2_staticBody : b2_dynamicBody;

			//defines the size of created body
			b2PolygonShape shape;
			shape.SetAsBox((width / 2) / SCALE, (height / 2) / SCALE);

			//gives body its density/weight and friction
			b2FixtureDef fixtureDef;
			fixtureDef.density = density;
			fixtureDef.friction = friction;
			fixtureDef.shape = &shape;

			//sets entity's rigidbody
			rigidbody.body = world->CreateBody(&bodyDef);
			rigidbody.body->CreateFixture(&fixtureDef);
		}

		/*
		void Velocity(Entity entity, float vx, float vy)
		{

			char moveState;
			//Enumeration for movements
			enum _moveState {
				MS_STOP,
				MS_LEFT,
				MS_RIGHT,
				MS_JUMP,
			};
			//sets movement speed. Instant movement, no gradual acceleration
			switch (moveState)
			{
			case MS_LEFT:  velocity.x = -vx; break;
			case MS_STOP:  velocity.x = 0; break;
			case MS_RIGHT: velocity.x = vx; break;
			//apply immediate force upwards
			case MS_JUMP: body->ApplyLinearImpulse(b2Vec2(0, vy)); break;
			}
			body->SetLinearVelocity(velocity);


		}
		*/

	private:
		b2Vec2 gravity = b2Vec2(0, 0);
		b2World* world;
		float timeStep = 1.0f / 60.0f;
		const float SCALE = 1; //magic number
		//b2Vec2 velocity = body->GetLinearVelocity();
	};
}
