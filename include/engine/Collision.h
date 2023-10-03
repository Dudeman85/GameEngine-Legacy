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

		//Draw the collider vertices
		bool visualize = false;
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

		//Checks collision between entity a and every other entity
		//Returns the collisions from the perspective of a
		vector<CollisionEvent> CheckCollision(Entity a)
		{
			Transform& aTransform = ecs.getComponent<Transform>(a);
			PolygonCollider& aCollider = ecs.getComponent<PolygonCollider>(a);

			//Rotate and scale every vertex of a, movement is handled later
			vector<Vector2> aVerts;
			for (int i = 0; i < aCollider.vertices.size(); i++)
			{
				Vector2 transformedVert = aCollider.vertices[i];
				//Rotate
				float angle = aTransform.rotation.z * PI / 180;
				transformedVert.x = aCollider.vertices[i].x * cos(angle) - aCollider.vertices[i].y * sin(angle);
				transformedVert.y = aCollider.vertices[i].x * sin(angle) + aCollider.vertices[i].y * cos(angle);
				//Scale
				transformedVert *= Vector2(aTransform.scale);
				aVerts.push_back(transformedVert);
			}

			//Store every collision between this entity and others
			std::vector<CollisionEvent> collisions;

			//For each entity
			for (const Entity& b : entities)
			{
				//Don't collide with self
				if (a == b)
					continue;

				//Check AABB collision first
				if (!AABBIntersect(a, b))
					continue;

				Transform& bTransform = ecs.getComponent<Transform>(b);
				PolygonCollider& bCollider = ecs.getComponent<PolygonCollider>(b);

				//Rotate and scale every vertex of b, movement is handled later
				vector<Vector2> bVerts;
				for (int i = 0; i < bCollider.vertices.size(); i++)
				{
					Vector2 transformedVert = bCollider.vertices[i];
					//Rotate
					float angle = bTransform.rotation.z * PI / 180;
					transformedVert.x = bCollider.vertices[i].x * cos(angle) - bCollider.vertices[i].y * sin(angle);
					transformedVert.y = bCollider.vertices[i].x * sin(angle) + bCollider.vertices[i].y * cos(angle);
					//Scale
					transformedVert *= Vector2(bTransform.scale);
					bVerts.push_back(transformedVert);
				}

				//Check collision from both a and b perspectives
				CollisionEvent aToB = SATIntersect(aVerts, bVerts, a, b);
				CollisionEvent bToA;
				//If a to b is miss no need to check b to a
				if (aToB.type != CollisionEvent::Type::miss)
				{
					bToA = SATIntersect(bVerts, aVerts, b, a);
					if (bToA.type != CollisionEvent::Type::miss)
						collisions.push_back(aToB);
				}
			}
			return collisions;
		}

		//Check SAT intersection
		//Expects Vertices to have Rotation and Scale applied, but not Position, this if for optimization
		CollisionEvent SATIntersect(vector<Vector2> aVerts, vector<Vector2> bVerts, Entity a, Entity b)
		{
			Vector2 aPosition = ecs.getComponent<Transform>(a).position;
			Vector2 bPosition = ecs.getComponent<Transform>(b).position;

			//For each vertice in b move it to have a at origin
			for (int i = 0; i < bVerts.size(); i++)
			{
				bVerts[i] += bPosition;
				bVerts[i] -= aPosition;
			}

			//For each vertice in a
			for (int i = 0; i < aVerts.size(); i++)
			{
				//Overflow nextVertice to beginning
				int nextVertice = i < aVerts.size() - 1 ? i + 1 : 0;
				//Calculate the normal vector to project the other polygon to (left normal because clockwise)
				Vector2 normal = (aVerts[nextVertice] - aVerts[i]).LeftNormal();
				//The boundry of the polygon projected to normal
				float boundary = normal.Dot(aVerts[i]);

				float miss = true;

				//For each vertice in b
				for (int j = 0; j < bVerts.size(); j++)
				{
					//Project the vertice of b to the normal of the current a vertice
					float projection = normal.Dot(bVerts[j]);
					//If the projection of the vertex is inside the boundary
					if (abs(projection) < abs(boundary))
					{
						miss = false;
					}
				}
				if (miss)
					return CollisionEvent(); //False
			}
			return CollisionEvent(); //true;
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
			std::array<float, 4> bounds{ -INFINITY, -INFINITY, INFINITY, INFINITY };
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