#include <iostream>
#include "engine/ECSCore.h"
#include <AL/al.h>
#include <chrono>

//Create one instance of the ecs manager
ECS ecs;

//Simple position and gravity Components
struct Position
{
	float x, y;
};
struct Gravity
{
	float x, y;
};

//System class, this holds every function of the GravitySystem system
class GravitySystem : public System
{
public:
	//Update the entity's position
	void Update()
	{
		//For each entity that has the required components
		for (auto const& entity : entities)
		{
			//Get the relevant components from entity
			Position& position = ecs.getComponent<Position>(entity);
			Gravity& gravity = ecs.getComponent<Gravity>(entity);

			//Update the entity's postion component
			position.y += gravity.y;
			position.x += gravity.x;
		}
	}
};

int main()
{
	//Register the gravity system, it is accessible by this pointer
	std::shared_ptr<GravitySystem> gravitySystem = ecs.registerSystem<GravitySystem>();

	//Add Position and Gravity components as requirements for GravitySystem system
	Signature gravitySystemSignature;
	gravitySystemSignature.set(ecs.getComponentId<Position>());
	gravitySystemSignature.set(ecs.getComponentId<Gravity>());
	ecs.setSystemSignature<GravitySystem>(gravitySystemSignature);

	//Create a new entity
	Entity player = ecs.newEntity();

	//Add the position component and set it's starting position
	ecs.addComponent(player, Position{.x = 4, .y = 10});
	//Add the gravity component and set it's direction
	ecs.addComponent(player, Gravity{ .x = 0, .y = -1.0f });

	while (true)
	{
		//Gravity system updates every entity with the required components
		gravitySystem->Update();

		std::cout << "Y: " << ecs.getComponent<Position>(player).y << ", X: " << ecs.getComponent<Position>(player).x << std::endl;
	}
}