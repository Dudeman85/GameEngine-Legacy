#pragma once
#include "engine/ECSCore.h"
#include "engine/Transform.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace engine;

struct Camera
{
	sf::View cam;
	float followDistance;
	Entity target;
};

class CameraSystem : public System
{
public:
	void Update()
	{
		for (auto const& entity : entities)
		{
			Camera& camera = ecs.getComponent<Camera>(entity);
			Transform& camPos = ecs.getComponent<Transform>(entity);
			Transform& follow = ecs.getComponent<Transform>(camera.target);

			if (camPos.x - follow.x > camera.followDistance)
			{
				std::cout << "go left";
				camPos.x += -5 * ((abs(follow.x - follow.x) - 200) / 30);
			}
			if (camPos.x - follow.x < camera.followDistance)
			{
				std::cout << "go right";
				camPos.x += 5 * ((abs(follow.x - follow.x) - 200) / 30);
			}
			if (camPos.y - follow.y > camera.followDistance)
			{
				std::cout << "go Up";
				camPos.y += -5 * ((abs(follow.y - follow.y) - 200) / 30);
			}
			if (camPos.y - follow.y < camera.followDistance)
			{
				std::cout << "go down";
				camPos.y += 5 * ((abs(follow.y - follow.y) - 200) / 30);
			}

			camera.cam.setCenter(camPos.x, camPos.y);
		}
	}

private:

};