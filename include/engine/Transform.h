#pragma once
#include "ECSCore.h"

extern ECS ecs;

namespace engine
{
	// Transform component
	struct Transform
	{
		float x, y, z;
		float xScale, yScale;
		float rotation;
	};

	// Transform system
	class TransformSystem : public System
	{
	public:
		void Update()
		{
			for (auto const& entity : entities)
			{

			}
		}

		//Set the absolute position of entity
		void setPosition(Entity entity, float x, float y, float z = 0)
		{
			Transform& transform = ecs.getComponent<Transform>(entity);
			transform.x = x;
			transform.y = y;
			transform.z = z;
		}
	};
}