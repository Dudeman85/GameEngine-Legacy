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

	//Physics System
	//Requires Rigidbody, Transform, and BoxCollider components
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem() {}

		void Update(float deltaTime)
		{
			//For each physics step per frame
			for (int i = 0; i < step; i++)
			{
				//For each entity
				for (auto const& entity : entities)
				{
					Transform& transform = ecs.getComponent<Transform>(entity);
					Rigidbody& rigidbody = ecs.getComponent<Rigidbody>(entity);
					BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);

					TransformSystem::Translate(entity, rigidbody.velocity);
				}
			}
		}

		int step = 4;
		Vector2 gravity;
	};

	//Check if an entity is colliding with a tile in a tilemap
	bool CheckTilemapCollision(Entity entity)
	{
		return false;
	}
}