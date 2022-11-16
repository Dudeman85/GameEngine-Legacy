#pragma once
#include <iostream>
#include <map>
#include <bitset>
#include <stack>
#include <map>
#include <set>
#include <memory>

//Max amount of unique entities and components, needed for array sizing
const uint16_t MAX_ENTITIES = 10000;
const uint16_t MAX_COMPONENTS = 100;

//Entity is only a numerical id
using Entity = uint16_t;
//The signature is a bitset with one bit per possible component
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
			return -1;
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

//Generic component array interface for component manager
class IComponentArray
{
public: virtual void removeComponent(Entity entity) = 0;
};

//Stores all the instances of a component type
template<typename T>
class ComponentArray : public IComponentArray
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

	void addComponent(Entity entity, T component)
	{
		//Update entity and index maps to include new entity
		entityToIndex[entity] = size;
		indexToEntity[size] = entity;

		//Add the component to the packed array
		componentArray[size] = component;

		size++;
	}

	void removeComponent(Entity entity) override
	{
		if (entityToIndex.find(entity) == entityToIndex.end())
		{
			std::cout << "Warning: Trying to remove non-existent component" << std::endl;
			return;
		}

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
		if (entityToIndex.find(entity) == entityToIndex.end())
		{
			throw("Error: Entity does not have component\n");
		}

		//Return a reference to entity's component
		return componentArray[entityToIndex[entity]];
	}
};

//Stores and manages every component array
class ComponentManager
{
public:
	uint16_t nextId = 0;

	//A map from a component type's name to its id
	std::map<const char*, uint16_t> typeToId;
	//A map from a component type's id to its name
	std::map<uint16_t, const char*> idToType;

	//A map from a component type's name to the component array of its type
	std::map<const char*, std::shared_ptr<IComponentArray>> componentArrays;

	//Register component type
	template<typename T>
	void registerComponent()
	{
		//Name of the new component's type
		const char* componentType = typeid(T).name();

		//Assigns an id and makes a new component array for the registered component type
		typeToId.insert({ componentType, nextId });
		idToType.insert({ nextId, componentType });
		componentArrays.insert({ componentType, std::make_shared<ComponentArray<T>>() });

		nextId++;
	}

	//Returns the id of a component type
	template<typename T>
	uint16_t getComponentId()
	{
		return typeToId[typeid(T).name()];
	}

	//Adds a component of type T to entity
	template<typename T>
	void addComponent(Entity entity, T component)
	{
		//Call the addComponent method of the correct component array
		getComponentArray<T>()->addComponent(entity, component);
	}

	//Removes a component of type T from entity
	template<typename T>
	void removeComponent(Entity entity)
	{
		//Call the removeComponent method of the correct component array
		getComponentArray<T>()->removeComponent(entity);
	}

	template<typename T>
	T& getComponent(Entity entity)
	{
		//Call the getComponent method of the correct component array
		return getComponentArray<T>()->getComponent(entity);
	}

	void destroyEntity(Entity entity, Signature signature)
	{
		for (size_t i = 0; i < signature.size(); i++)
		{
			if (signature[i] == 0)
			{
				componentArrays[idToType[i]]->removeComponent(entity);
			}
		}
	}

private:
	//QOL function to get the casted ComponentArray
	template<typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray()
	{
		const char* componentType = typeid(T).name();

		//If the component has not been registered, do it
		if (componentArrays.find(componentType) == componentArrays.end())
		{
			registerComponent<T>();
		}

		//Return a Cast ComponentArray of desired type
		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[componentType]);
	}
};

//Base class all systems inherit from
class System
{
public:
	//Set of every entity containing the required components for the system
	std::set<Entity> entities;
};

//Manager class to make sure every system has the correct list of entitites
class SystemManager
{
public:
	//Map of all systems accessible by their type names
	std::map<const char*, std::shared_ptr<System>> systems;
	//Map of each system's signature accessible by their type name
	std::map<const char*, Signature> systemSignatures;

	template<typename T>
	std::shared_ptr<T> registerSystem()
	{
		//Name of the new system's type
		const char* systemType = typeid(T).name();

		//Create new system and return a pointer to it
		std::shared_ptr<T> system = std::make_shared<T>();
		systems.insert({ systemType, (system) });
		return system;
	}

	//Sets the signature (required components) for the system
	template<typename T>
	void setSignature(Signature signature)
	{
		//Name of the new system's type
		const char* sytemType = typeid(T).name();

		systemSignatures.insert({ sytemType, signature });
	}

	void destroyEntity(Entity entity)
	{
		//Loop through each system and remove the destoyed entity
		for (auto const& system : systems)
		{
			system.second->entities.erase(entity);
		}
	}

	void onEntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		//Loop through every system
		for (auto const& system : systems)
		{
			//Bitwise and to check if the entity contains all the required components for the system
			if ((entitySignature & systemSignatures[system.first]) == systemSignatures[system.first])
			{
				//Add the entity to the system's set
				system.second->entities.insert(entity);
			}
			else
			{
				//Remove the entity from the system's set
				system.second->entities.erase(entity);
			}
		}
	}
};

//General ECS manager class to interface with the entire ECS framework
class ECS
{
public:
	EntityManager* entityManager;
	ComponentManager* componentManager;
	SystemManager* systemManager;

	ECS()
	{
		componentManager = new ComponentManager();
		entityManager = new EntityManager();
		systemManager = new SystemManager();
	}
	~ECS()
	{
		delete componentManager;
		delete entityManager;
		delete systemManager;
	}

	//Returns a newly created entity with nothing attached
	Entity newEntity()
	{
		return entityManager->newEntity();
	}

	//Destroys an entity and all of its components
	void destroyEntity(Entity entity)
	{
		entityManager->deleteEntity(entity);
		componentManager->destroyEntity(entity, entityManager->entitySignatures[entity]);
		systemManager->destroyEntity(entity);
	}

	//Register a component
	template<typename T>
	void registerComponent()
	{
		componentManager->registerComponent<T>();
	}

	//Add a component to an entity
	template<typename T>
	void addComponent(Entity entity, T component)
	{
		componentManager->addComponent(entity, component);
		entityManager->entitySignatures[entity].set(componentManager->getComponentId<T>(), true);
		systemManager->onEntitySignatureChanged(entity, entityManager->entitySignatures[entity]);
	}

	//Remove a component from an entity
	template<typename T>
	void removeComponent(Entity entity)
	{
		componentManager->removeComponent<T>(entity);
		entityManager->entitySignatures[entity].set(componentManager->getComponentId<T>(), false);
		systemManager->onEntitySignatureChanged(entity, entityManager->entitySignatures[entity]);
	}

	//Returns a pointer to the desired component of the entity
	template<typename T>
	T& getComponent(Entity entity)
	{
		return componentManager->getComponent<T>(entity);
	}

	//Returns the id of the component type
	template<typename T>
	uint16_t getComponentId()
	{
		return componentManager->getComponentId<T>();
	}

	//Registers a system, returns a pointer to that system
	//Every system needs to be registered before it can be used
	template<typename T>
	std::shared_ptr<T> registerSystem()
	{
		return systemManager->registerSystem<T>();
	}

	//Sets the signature of required components for a system
	template<typename T>
	void setSystemSignature(Signature signature)
	{
		systemManager->setSignature<T>(signature);
	}
}; 