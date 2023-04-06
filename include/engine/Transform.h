#pragma once
#include <engine/ECSCore.h>

extern ECS ecs;

namespace engine
{
	struct Vector2;

	//Three floats in one
	struct Vector3
	{
		Vector3() 
		{
			x = 0;
			y = 0;
			z = 0;
		}
		Vector3(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}
		Vector3(Vector2 vec2, float _z = 0);

		float x, y, z;
	};

	//Two floats in one
	struct Vector2
	{
		Vector2()
		{
			x = 0;
			y = 0;
		}
		Vector2(float _x, float _y)
		{
			x = _x;
			y = _y;
		}
		Vector2(Vector3 vec3)
		{
			x = vec3.x;
			y = vec3.y;
		}

		float x, y;
	};

	//I really should have used cpp files
	Vector3::Vector3(Vector2 vec2, float _z)
	{
		x = vec2.x;
		y = vec2.y;
		z = _z;
	}

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
	};
}