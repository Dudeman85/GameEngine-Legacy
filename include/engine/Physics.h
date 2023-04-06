#pragma once
#include <engine/ECSCore.h>
#include <engine/Transform.h>

namespace engine
{
	//Rigidbody component
	struct Rigidbody
	{
		Vector2 velocity;
		float drag = 0;
		bool enabled = true;
		bool isStatic = false;
	};

	//Box collider component
	struct BoxCollider
	{
		Vector2 scale = Vector2(1, 1);
		Vector2 offset = Vector2(0, 0);
		bool isTrigger = false;
	};

	//Collision struct, not a component
	struct Collision
	{
		Entity a;
		Entity b;

		bool collided;
		bool isTrigger;

		//How much entity b has overlapped entity a
		float topIntersect;
		float bottomIntersect;
		float leftIntersect;
		float rightIntersect;
	};

	//Physics System
	//Requires Rigidbody, Transform, and BoxCollider components
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem(float gravityX = 0, float gravityY = 0)
		{
			gravity.x = gravityX;
			gravity.y = gravityY;
		}

		void Update(float deltaTime)
		{
			//For each physics step per frame
			for (int i = 0; i < step; i++)
			{
				//For each entity
				for (auto const& entity : entities)
				{
					//Get required components
					Transform& transform = ecs.getComponent<Transform>(entity);
					Rigidbody& rigidbody = ecs.getComponent<Rigidbody>(entity);
					BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);

					//Dont process static rigidbodies
					if (rigidbody.isStatic)
						continue;

					//Apply gravity
					rigidbody.velocity += gravity / step * deltaTime;

					//Apply drag
					rigidbody.velocity -= rigidbody.velocity * rigidbody.drag / step * deltaTime;

					cout << rigidbody.velocity.y << endl;

					//Move the entity
					Move(entity, rigidbody.velocity / step * deltaTime);
				}
			}
		}

		//Move an entity while checking collision at every step
		//If a collision occurs returns the step it occured on, entity is also moved up to the collision
		int Move(Entity entity, Vector2 direction, int stepOverride = 0)
		{
			Rigidbody& rigidbody = ecs.getComponent<Rigidbody>(entity);

			if (stepOverride == 0)
				stepOverride = step;

			//For each physics step
			for (int i = 0; i < stepOverride; i++)
			{
				//Move the entity
				TransformSystem::Translate(entity, direction / step);

				//Perform collision detection
				vector<Collision> collisions = DetectCollision(entity);

				//Move the entity back so it is no longer colliding
				for (const Collision& collision : collisions)
				{
					if (collision.isTrigger)
						continue;

					//Get the smallest intersection amount 
					//This can be done better
					//TODO FIX BUG: when two intersects are the same nothing is moved
					float maxIntersect = max(collision.topIntersect, max(collision.bottomIntersect, max(collision.leftIntersect, collision.rightIntersect)));
					if (collision.topIntersect != maxIntersect && collision.topIntersect != 0)
					{
						//Move down
						TransformSystem::Translate(entity, 0, -collision.topIntersect);
						rigidbody.velocity.y = 0;
					}
					else if (collision.bottomIntersect != maxIntersect && collision.bottomIntersect != 0)
					{
						//Move up
						TransformSystem::Translate(entity, 0, collision.bottomIntersect);
						rigidbody.velocity.y = 0;
					}
					else if (collision.leftIntersect != maxIntersect && collision.leftIntersect != 0)
					{
						//Move right
						TransformSystem::Translate(entity, collision.leftIntersect, 0);
						rigidbody.velocity.x = 0;
					}
					else
					{
						//Move left
						TransformSystem::Translate(entity, -collision.rightIntersect, 0);
						rigidbody.velocity.x = 0;
					}
				}
				if (collisions.size() != 0)
					return i + 1;
			}
			return 0;
		}

		//Performs AABB collision detection between a and every other entity with a collider
		vector<Collision> DetectCollision(Entity a)
		{
			vector<Collision> collisions;
			//For each entity
			for (auto const& b : entities)
			{
				if (a == b)
					continue;

				//Check the intersect, if it collided add it to the list
				Collision collision = AABBIntersect(a, b);
				if (collision.collided)
				{
					collisions.push_back(collision);
				}
			}
			return collisions;
		}

		//Returns a Collision containing data about the axis-aligned bounding box intersects of a and b
		static Collision AABBIntersect(Entity a, Entity b)
		{
			//Get necessary components for entity a and b
			Transform& aTransform = ecs.getComponent<Transform>(a);
			BoxCollider& aCollider = ecs.getComponent<BoxCollider>(a);
			Transform& bTransform = ecs.getComponent<Transform>(b);
			BoxCollider& bCollider = ecs.getComponent<BoxCollider>(b);

			//Get the min and max bounds for entity a
			Vector2 aMin;
			aMin.x = aTransform.x - aTransform.xScale * aCollider.scale.x + aCollider.offset.x;
			aMin.y = aTransform.y - aTransform.yScale * aCollider.scale.y + aCollider.offset.y;
			Vector2 aMax;
			aMax.x = aTransform.x + aTransform.xScale * aCollider.scale.x + aCollider.offset.x;
			aMax.y = aTransform.y + aTransform.yScale * aCollider.scale.y + aCollider.offset.y;

			//Get the min and max bounds for entity b
			Vector2 bMin;
			bMin.x = bTransform.x - bTransform.xScale * bCollider.scale.x + bCollider.offset.x;
			bMin.y = bTransform.y - bTransform.yScale * bCollider.scale.y + bCollider.offset.y;
			Vector2 bMax;
			bMax.x = bTransform.x + bTransform.xScale * bCollider.scale.x + bCollider.offset.x;
			bMax.y = bTransform.y + bTransform.yScale * bCollider.scale.y + bCollider.offset.y;

			Collision collision{ .a = a, .b = b, .collided = false };

			//Perform AABB intersect
			if (aMin.x < bMax.x && aMax.x > bMin.x && aMin.y < bMax.y && aMax.y > bMin.y)
			{
				collision.collided = true;
				collision.isTrigger = aCollider.isTrigger || bCollider.isTrigger;

				//Calculate the intersect amounts
				collision.topIntersect = aMax.y - bMin.y;
				collision.bottomIntersect = bMax.y - aMin.y;
				collision.leftIntersect = bMax.x - aMin.x;
				collision.rightIntersect = aMax.x - bMin.x;

				//Cull the backwards intersects
				if (collision.leftIntersect < collision.rightIntersect)
					collision.rightIntersect = 0;
				else
					collision.leftIntersect = 0;
				if (collision.topIntersect < collision.bottomIntersect)
					collision.bottomIntersect = 0;
				else
					collision.topIntersect = 0;
			}

			return collision;
		}

		int step = 4;
		//Pixels/second^2
		Vector2 gravity;

		//Check if an entity is colliding with a tile in a tilemap
		static bool CheckTilemapCollision(Entity entity)
		{
			return false;
		}
	};
}