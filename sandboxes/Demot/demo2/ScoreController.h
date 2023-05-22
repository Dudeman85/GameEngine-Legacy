#pragma once
#include <engine/Application.h>
#include "PickupController.h";

extern ECS ecs;
using namespace engine;

class ScoreController : public System
{
public:
	ScoreController(int& collectedRef)
	{
		collectedRef = PickupController().collected;
		std::cout << collectedRef << endl;
	}


};