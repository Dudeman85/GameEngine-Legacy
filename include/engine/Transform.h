#pragma once
#include <glm/glm.hpp>
#include <engine/ECSCore.h>
#include <engine/Vector.h>

extern ECS ecs;

namespace engine
{
	//Transform component
	struct Transform
	{
		Vector3 position;
		Vector3 rotation;
		Vector3 scale = Vector3(1.0f);
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
			transform.position.x += dx;
			transform.position.y += dy;
			transform.position.z += dz;
		}
		//Translate an entity by dt
		static void Translate(Entity entity, Vector3 dt)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			transform.position += dt;
		}

		//Set the absolute position of entity
		static void SetPosition(Entity entity, float x, float y, float z = 0)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			transform.position.x = x;
			transform.position.y = y;
			transform.position.z = z;
		}
		//Set the absolute position of entity
		static void SetPosition(Entity entity, Vector3 position)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			transform.position = position;
		}
		//Get the distance between two entities
		static float Distance(Entity a, Entity b)
		{
			Transform& aTransform = ecs.getComponent<Transform>(a);
			Transform& bTransform = ecs.getComponent<Transform>(b);

			return sqrt(pow(bTransform.position.x - aTransform.position.x, 2) + pow(bTransform.position.y - aTransform.position.y, 2) + pow(bTransform.position.z - aTransform.position.z, 2));
		}

		//Get the angle of b with a as the origin
		//TODO Fix this to work in 3D
		static float Angle(Entity a, Entity b)
		{
			Transform& aTransform = ecs.getComponent<Transform>(a);
			Transform& bTransform = ecs.getComponent<Transform>(b);

			return glm::degrees(atan2(bTransform.position.y - aTransform.position.y, bTransform.position.x - aTransform.position.x));
		}
	};
}