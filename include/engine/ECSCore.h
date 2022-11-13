#pragma once
#include <map>
#include <bitset>
#include <stack>
#include <map>

const uint16_t MAX_ENTITIES = 10000;
const uint16_t MAX_COMPONENTS = 100;

using Entity = uint16_t;
using Signature = std::bitset<MAX_COMPONENTS>;

//Class for managing entities
class EntityManager
{
public:
	std::stack<Entity> availableEntities;
	Signature entitySignatures[MAX_ENTITIES];
	uint16_t entityCount = 0;

	EntityManager()
	{
		//Initialize availeableEntityIds to include every possible id
		for (size_t i = 0; i < MAX_ENTITIES; i++)
		{
			availableEntities.push(i);
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

		Entity e = availableEntities.top();
		availableEntities.pop();

		return e;
	}

	//Set the entity id as available
	void deleteEntity(Entity e)
	{
		entitySignatures[e].reset();
		entityCount--;
		availableEntities.push(e);
	}
};
 
//Stores all the instances of a component type
template<typename T>
class ComponentArray
{
public:
	//Packed array of all components of type T
	T componentArray[MAX_ENTITIES];
	
	//Map from an entity id to the index of its component in the componentArray
	std::map<Entity, int> entityToIndex;
	//Map from a components index in the componentArray to its entity's id
	std::map<int, Entity> indexToEntity;
	
	//Amount of components in existance
	int size = 0;

	int componentId;

	void addComponent(Entity entity, T component)
	{
		//Update entity and index maps to include new entity
		entityToIndex[entity] = size;
		indexToEntity[size] = entity;

		//Add the component to the packed array
		componentArray[size] = component;
		
		size++;
	}

	void removeComponent(Entity entity)
	{
		//Keep track of the deleted components index, and the entity of the last component in the array
		int deletedIndex = entityToIndex[entity];
		Entity lastEntity = indexToEntity[size - 1];

		//Overwrite the deleted component by moving the last component in the component array to its index
		componentArray[deletedIndex] = componentArray[size - 1];

		//Update the maps of the moved component
		entityToIndex[lastEntity] = deletedIndex;
		indexToEntity[deletedIndex] = lastEntity;

		//Remove the deleted entity, and moved component from the maps
		entityToIndex.erase(entity);
		indexToEntity.erase(size - 1);
	}

	T& getComponent(Entity entity)
	{
		//Return a reference to entity's component
		return componentArray[entityToIndex[entity]];
	}
};

class ComponentManager
{

};

class ECS
{

};