#pragma once
#include <engine/Vector.h>
#include <engine/ECSCore.h>
#include <vector>
#include <array>

extern ECS ecs;

namespace engine
{
	//Polygon Collider component
	struct PolygonCollider
	{
		//The vertices of the polygon making up the collider, going clockwise
		//The vertices must form a convex polygon
		std::vector<Vector2> vertices;
		
		//The axis-aligned bounding box of the collider
		//This is updated automatically and is set in world coordinates
		std::array<float, 4> bounds;
	};

	//Collision Event struct, not a component
	struct CollisionEvent
	{
		enum class Type { collision, trigger, miss };

		Type type;

		//The entity which instigated the collision
		Entity a;
		//The entity which was subject to the collision
		Entity b;
	};

	//Collision System
	//Requires Transform and PolygonCollider
	class CollisionSystem : public System
	{
	public:
		void Update()
		{
			//For each entity
			for (const Entity& entity : entities)
			{

			}
		}

		//Checks collision between entity and every other entity
		vector<CollisionEvent> CheckCollision(Entity entity)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			PolygonCollider& collider = ecs.getComponent<PolygonCollider>(entity);

			//For each entity
			for (const Entity& entity2 : entities)
			{
				//Don't collide with self
				if (entity == entity2)
					continue;

				//Check AABB collision first
				if (!AABBIntersect(entity, entity2))
					continue;
			}

			return std::vector<CollisionEvent>();
		}

		//Checks if a and b bounds are intersecting
		bool AABBIntersect(Entity a, Entity b)
		{
			//Get the bounds
			std::array<float, 4>& aBounds = ecs.getComponent<PolygonCollider>(a).bounds;
			std::array<float, 4>& bBounds = ecs.getComponent<PolygonCollider>(b).bounds;

			//Perform AABB intersect
			return (aBounds[3] < bBounds[1] && aBounds[1] > bBounds[3] && aBounds[2] < bBounds[0] && aBounds[0] > bBounds[2]);
		}

		//Update the AABB of the polygon collider
		void UpdateAABB(Entity entity)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			PolygonCollider& collider = ecs.getComponent<PolygonCollider>(entity);

			//Bounds go top, right, bottom, left
			std::array<float, 4> bounds{-INFINITY, -INFINITY, INFINITY, INFINITY};
			//For each vertice apply the tranform and calculate min and max bounds
			for (int i = 0; i < collider.vertices.size(); i++)
			{
				//Apply transform to the polygon collider
				Vector2 transformedVert = collider.vertices[i];
				//Rotate
				float angle = transform.rotation.z * PI / 180;
				transformedVert.x = collider.vertices[i].x * cos(angle) - collider.vertices[i].y * sin(angle);
				transformedVert.y = collider.vertices[i].x * sin(angle) + collider.vertices[i].y * cos(angle);
				//Scale
				transformedVert *= Vector2(transform.scale);
				//Move
				transformedVert += transform.position;

				//Check bounds
				//Top bound
				if (transformedVert.y > bounds[0])
					bounds[0] = transformedVert.y;
				//Right bound
				if (transformedVert.x > bounds[1])
					bounds[1] = transformedVert.x;
				//Bottom bound
				if (transformedVert.y < bounds[2])
					bounds[2] = transformedVert.y;
				//Left bound
				if (transformedVert.x < bounds[3])
					bounds[3] = transformedVert.x;
			}
			
			collider.bounds = bounds;
		}
	};
}