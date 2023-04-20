#pragma once
#include <engine/ECSCore.h>
#include <engine/Transform.h>
#include <engine/Tilemap.h>

namespace engine
{
	//Collision struct, not a component
	struct Collision
	{
		//The entity which instigated the collision
		Entity a;
		//The entity which was subject to the collision
		Entity b;

		bool collided;
		bool isTrigger;
		unsigned int tileID;

		//How much entity b has overlapped entity a on each side
		//Order is top, right, bottom, left
		float intersects[4];
	};

	//Rigidbody component
	struct Rigidbody
	{
		Vector2 velocity;
		float drag = 0;
		float gravityScale = 1;
		float friction = 1;
		float elasticity = 0;
		bool isStatic = false;
	};

	//Box collider component
	struct BoxCollider
	{
		Vector2 scale = Vector2(1, 1);
		Vector2 offset = Vector2(0, 0);
		bool isTrigger = false;
		vector<Collision> collisions;
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

					if (i == 0)
						collider.collisions.clear();

					//Dont process static rigidbodies
					if (rigidbody.isStatic)
						continue;

					//Apply gravity
					rigidbody.velocity += gravity * rigidbody.gravityScale / step * deltaTime;

					//Apply drag
					rigidbody.velocity -= rigidbody.velocity * rigidbody.drag / step * deltaTime;

					//Move the entity
					Move(entity, rigidbody.velocity / step * deltaTime, 1);
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

					Rigidbody& collisionRigidbody = ecs.getComponent<Rigidbody>(collision.b);

					//Get the smallest intersection amount 
					float minIntersect = INFINITY;
					int side = 0;
					for (int i = 0; i < 4; i++)
					{
						//0 means no intersection on that side
						if (collision.intersects[i] == 0)
							continue;

						if (collision.intersects[i] < minIntersect)
						{
							minIntersect = collision.intersects[i];
							side = i;
						}
					}

					switch (side)
					{
					case 0:
						//Collision on top, move down
						TransformSystem::Translate(entity, 0, -collision.intersects[0]);
						//Apply friction and elasticity to appropriate axis
						rigidbody.velocity.x -= rigidbody.velocity.x * ((rigidbody.friction + collisionRigidbody.friction) / 2);
						rigidbody.velocity.y = -rigidbody.velocity.y * ((rigidbody.elasticity + collisionRigidbody.elasticity) / 2);
						break;
					case 1:
						//Collision on right, move left
						TransformSystem::Translate(entity, -collision.intersects[1], 0);
						//Apply friction and elasticity to appropriate axis
						rigidbody.velocity.x = -rigidbody.velocity.x * ((rigidbody.elasticity + collisionRigidbody.elasticity) / 2);
						rigidbody.velocity.y -= rigidbody.velocity.y * ((rigidbody.friction + collisionRigidbody.friction) / 2);
						break;
					case 2:
						//Collision on bottom, move up
						TransformSystem::Translate(entity, 0, collision.intersects[2]);
						//Apply friction and elasticity to appropriate axis
						rigidbody.velocity.x -= rigidbody.velocity.x * ((rigidbody.friction + collisionRigidbody.friction) / 2);
						rigidbody.velocity.y = -rigidbody.velocity.y * ((rigidbody.elasticity + collisionRigidbody.elasticity) / 2);
						break;
					case 3:
						//Collision on left, move right
						TransformSystem::Translate(entity, collision.intersects[3], 0);
						//Apply friction and elasticity to appropriate axis
						rigidbody.velocity.x = -rigidbody.velocity.x * ((rigidbody.elasticity + collisionRigidbody.elasticity) / 2);
						rigidbody.velocity.y -= rigidbody.velocity.y * ((rigidbody.friction + collisionRigidbody.friction) / 2);
						break;
					}
				}

				//If there was a collision don't process any more steps and return the current step
				if (collisions.size() != 0)
					return i + 1;
			}
			//No collision, return 0
			return 0;
		}

		//Performs AABB collision detection between a and every other entity with a collider as well as the tilemap
		vector<Collision> DetectCollision(Entity a)
		{
			Transform& aTransform = ecs.getComponent<Transform>(a);
			BoxCollider& aCollider = ecs.getComponent<BoxCollider>(a);

			vector<Collision> collisions;
			//For each entity
			for (auto const& b : entities)
			{
				if (a == b)
					continue;

				BoxCollider& bCollider = ecs.getComponent<BoxCollider>(b);

				//Check the intersect, if it collided add it to the list
				Collision collision = AABBIntersect(a, b);
				if (collision.collided)
				{
					collisions.push_back(collision);
					//Log the collision in both entity a and b
					aCollider.collisions.push_back(collision);
					bCollider.collisions.push_back(collision);
				}
			}

			//Tilemap
			vector<float> bounds = GetBounds(a);
			vector<unsigned int> tilemapCollisions;
			//Top-Right
			tilemapCollisions.push_back(tilemap->checkCollision(bounds[1], bounds[0]));
			//Bottom-Right
			tilemapCollisions.push_back(tilemap->checkCollision(bounds[1], bounds[2]));
			//Bottom-Left
			tilemapCollisions.push_back(tilemap->checkCollision(bounds[3], bounds[2]));
			//Top-Left
			tilemapCollisions.push_back(tilemap->checkCollision(bounds[3], bounds[0]));
			/*
			//TODO make more checks if collider is bigger than tile
			if (tilemapCollision != 0)
			{
				Collision collision{ .a = a, .b = 0, .collided = true, .isTrigger = false, .tileID = tilemapCollision };

				aCollider.collisions.push_back(collision);
			}*/

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
			vector<float> aBounds = GetBounds(a);
			Vector2 aMax(aBounds[1], aBounds[0]);
			Vector2 aMin(aBounds[3], aBounds[2]);

			//Get the min and max bounds for entity b
			vector<float> bBounds = GetBounds(b);
			Vector2 bMax(bBounds[1], bBounds[0]);
			Vector2 bMin(bBounds[3], bBounds[2]);

			Collision collision{ .a = a, .b = b, .collided = false };

			//Perform AABB intersect
			if (aMin.x < bMax.x && aMax.x > bMin.x && aMin.y < bMax.y && aMax.y > bMin.y)
			{
				collision.collided = true;
				collision.isTrigger = aCollider.isTrigger || bCollider.isTrigger;

				//Calculate the intersect amounts
				collision.intersects[0] = aMax.y - bMin.y;
				collision.intersects[1] = aMax.x - bMin.x;
				collision.intersects[2] = bMax.y - aMin.y;
				collision.intersects[3] = bMax.x - aMin.x;

				//Cull the backwards intersects
				if (collision.intersects[3] < collision.intersects[1])
					collision.intersects[1] = 0;
				else
					collision.intersects[3] = 0;
				if (collision.intersects[0] < collision.intersects[2])
					collision.intersects[2] = 0;
				else
					collision.intersects[0] = 0;
			}

			return collision;
		}

		//Get the bounds of the entity's collider
		//Order is top, right, bottom, left. Aka yMax, xMax, yMin, xMin
		static vector<float> GetBounds(Entity entity)
		{
			BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);
			Transform& transform = ecs.getComponent<Transform>(entity);

			vector<float> bounds;

			//Top
			bounds.push_back(transform.y + transform.yScale * collider.scale.y + collider.offset.y);
			//Right
			bounds.push_back(transform.x + transform.xScale * collider.scale.x + collider.offset.x);
			//Bottom
			bounds.push_back(transform.y - transform.yScale * collider.scale.y + collider.offset.y);
			//Right
			bounds.push_back(transform.x - transform.xScale * collider.scale.x + collider.offset.x);

			return bounds;
		}

		//Set a tilmap to check collision with
		void SetTilemap(Tilemap* map)
		{
			tilemap = map;
		}

		int step = 4;
		//Pixels/second^2
		Vector2 gravity;

	private:
		Tilemap* tilemap;
	};
}