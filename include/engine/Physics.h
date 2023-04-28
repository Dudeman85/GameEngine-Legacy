#pragma once
#include <engine/ECSCore.h>
#include <engine/Transform.h>
#include <engine/Tilemap.h>
#include <vector>

namespace engine
{
	//Collision struct, not a component
	struct Collision
	{
		enum Type { entity, tilemap, trigger, miss };

		Type type;

		//The entity which instigated the collision
		Entity a;
		//The entity which was subject to the collision
		Entity b;

		unsigned int tileID;

		//How much collider b has overlapped collider a on each side
		//Order is top, right, bottom, left
		std::array<float, 4> intersects;
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
					//Don't process triggers or misses
					if (collision.type == Collision::trigger || collision.type == Collision::miss)
						continue;

					Rigidbody collisionRigidbody;
					//Fake the Rigidbody of a tilemap to get friction and elasticity values
					if (collision.type == Collision::tilemap)
						collisionRigidbody = Rigidbody{ .friction = 0, .elasticity = 0 };
					else
						collisionRigidbody = ecs.getComponent<Rigidbody>(collision.b);

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

		//Performs AABB collision detection between a and every other entity with a collider as well as the tilemap if it exists
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
				if (collision.type != Collision::miss)
				{
					collisions.push_back(collision);
					//Log the collision in both entity a and b
					aCollider.collisions.push_back(collision);
					bCollider.collisions.push_back(collision);
				}
			}

			//Tilemap collision checking
			if (!tilemap)
				return collisions;

			std::array<float, 4> bounds = GetBounds(a);

			//TODO automate making more checks if collider is bigger than tile
			//Points to check at top-right, bottom-right, bottom-left, and top-left of the entity
			vector<Vector2> checkPoints{ Vector2(bounds[1], bounds[0]), Vector2(bounds[1], bounds[2]), Vector2(bounds[3], bounds[2]), Vector2(bounds[3], bounds[0]) };

			//Containers to store the necessary information only from hits
			vector<Vector2> collisionPoints;
			vector<Vector2> loggedTiles;

			//Check every point for a collision
			for (int i = 0; i < checkPoints.size(); i++)
			{
				unsigned int result = tilemap->checkCollision(checkPoints[i].x, checkPoints[i].y);

				//If the collision was a hit
				if (result != 0)
				{
					//Get the collision tile index
					Vector2 tileIndex(floor(checkPoints[i].x / tilemap->tileSize.x), floor(-checkPoints[i].y / tilemap->tileSize.y));

					//If that tile has not yet been logged as a collision
					if (find(loggedTiles.begin(), loggedTiles.end(), tileIndex) == loggedTiles.end())
					{
						Collision collision{ .type = Collision::tilemap, .a = a, .tileID = result };

						//Calculate the bounds for the collided tile
						std::array<float, 4> tileBounds{
							-tileIndex.y * tilemap->tileSize.y,
							(tileIndex.x + 1) * tilemap->tileSize.x,
							-(tileIndex.y + 1) * tilemap->tileSize.y,
							tileIndex.x * tilemap->tileSize.x };

						collision.intersects = GetIntersects(bounds, tileBounds);

						aCollider.collisions.push_back(collision);
						collisions.push_back(collision);
					}
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

			//Get the min and max bounds for entities a and b
			std::array<float, 4> aBounds = GetBounds(a);
			std::array<float, 4> bBounds = GetBounds(b);

			Collision collision{ .type = Collision::miss, .a = a, .b = b };

			//Perform AABB intersect
			if (aBounds[3] < bBounds[1] && aBounds[1] > bBounds[3] && aBounds[2] < bBounds[0] && aBounds[0] > bBounds[2])
			{
				//Set the collision data
				collision.type = aCollider.isTrigger || bCollider.isTrigger ? Collision::trigger : Collision::entity;
				collision.intersects = GetIntersects(aBounds, bBounds);
			}

			return collision;
		}

		//Get the bounds of the entity's collider
		//Order is top, right, bottom, left. Aka yMax, xMax, yMin, xMin
		static std::array<float, 4> GetBounds(Entity entity)
		{
			BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);
			Transform& transform = ecs.getComponent<Transform>(entity);

			std::array<float, 4> bounds;

			//Top
			bounds[0] = transform.y + transform.yScale * collider.scale.y + collider.offset.y;
			//Right
			bounds[1] = transform.x + transform.xScale * collider.scale.x + collider.offset.x;
			//Bottom
			bounds[2] = transform.y - transform.yScale * collider.scale.y + collider.offset.y;
			//Right
			bounds[3] = transform.x - transform.xScale * collider.scale.x + collider.offset.x;

			return bounds;
		}

		//Returns the amount of intersect between box a and box b
		//The order is top, right, bottom, left
		static std::array<float, 4> GetIntersects(std::array<float, 4> aBounds, std::array<float, 4> bBounds)
		{
			std::array<float, 4> intersects{
				aBounds[0] - bBounds[2],
				aBounds[1] - bBounds[3],
				bBounds[0] - aBounds[2],
				bBounds[1] - aBounds[3] };

			//Cull the backwards intersects
			if (intersects[3] < intersects[1])
				intersects[1] = 0;
			else
				intersects[3] = 0;
			if (intersects[0] < intersects[2])
				intersects[2] = 0;
			else
				intersects[0] = 0;

			return intersects;
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
		Tilemap* tilemap = nullptr;
	};
}