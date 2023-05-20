#pragma once
#include <engine/ECSCore.h>
#include <engine/Vector.h>

extern ECS ecs;

namespace engine
{
	//Transform component
	struct Transform
	{
		float x, y, z;
		float xScale, yScale, zScale;
		float xRotation, yRotation, zRotation;
	};

	//Transform system
	//Requires Transform component
	class TransformSystem : public System
	{
	public:
		//No update functionality yet
		void Update()
		{
			for (auto const& entity : entities)
			{

			}
		}

		//Translate an entity by dx, dy, and dz
		static void Translate(Entity entity, float dx, float dy, float dz = 0)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			transform.x += dx;
			transform.y += dy;
			transform.z += dz;
		}
		//Translate an entity by dt
		static void Translate(Entity entity, Vector3 dt)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			transform.x += dt.x;
			transform.y += dt.y;
			transform.z += dt.z;
		}

		//Set the absolute position of entity
		static void SetPosition(Entity entity, float x, float y, float z = 0)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			transform.x = x;
			transform.y = y;
			transform.z = z;
		}
		//Set the absolute position of entity
		static void SetPosition(Entity entity, Vector3 position)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			transform.x = position.x;
			transform.y = position.y;
			transform.z = position.z;
		}
		//Get the distance between two entities
		static float Distance(Entity a, Entity b)
		{
			Transform& aTransform = ecs.getComponent<Transform>(a);
			Transform& bTransform = ecs.getComponent<Transform>(b);

			return sqrt(pow(bTransform.x - aTransform.x, 2) + pow(bTransform.y - aTransform.y, 2));
		}
	};
}