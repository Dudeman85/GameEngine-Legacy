#pragma once
#include "ECSCore.h"
#include <box2d/box2d.h>

ECS ecs;
struct Position
{
	float x, y;
};
struct Gravity
{
	float x, y;
};

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