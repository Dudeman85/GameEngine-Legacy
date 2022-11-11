#pragma once
#include <map>
#include <bitset>
#include <stack>

namespace ecs 
{
	const uint16_t MAX_ENTITIES = 10000;
	const uint8_t MAX_COMPONENTS = 100;

	using Entity = uint16_t;
	using Signature = std::bitset<MAX_COMPONENTS>;

	//Class for managing entities. Should create one of these to manage all entities
	class EntityManager
	{
	public:
		std::stack<Entity> availableEntityIds;
		Signature entitySignatures[MAX_ENTITIES];
		uint16_t entityCount = 0;

		EntityManager()
		{
			//Initialize availeableEntityIds to include every possible id
			for (size_t i = 0; i < MAX_ENTITIES; i++)
			{
				availableEntityIds.push(i);
			}
		}

		//Return a unique entity id
		Entity newEntity()
		{
			if (entityCount >= MAX_ENTITIES)
			{
				std::cout << "Too Many Entities!" << std::endl;
				return NAN;
			}

			entityCount++;

			Entity e = availableEntityIds.top();
			availableEntityIds.pop();

			return e;
		}

		//Set the entity id as available
		void deleteEntity(Entity e)
		{
			entitySignatures[e].reset();
			entityCount--;
			availableEntityIds.push(e);
		}
	};
}
