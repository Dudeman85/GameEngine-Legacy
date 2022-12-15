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
		//For each entity
		for (auto const& entity : entities)
		{
			//Get relevant components
			Camera& camera = ecs.getComponent<Camera>(entity);
			Transform& camPos = ecs.getComponent<Transform>(entity);
			Transform& follow = ecs.getComponent<Transform>(camera.target);

			//Move the camera so that it follows the entity at followDistance, the speed increases the farther away the camera is from the entity making smooth motion
			//Left
			if (camPos.x - follow.x > camera.followDistance)
			{
				camPos.x += -5 * ((abs(camPos.x - follow.x) - camera.followDistance) / 30);
			}
			//Right
			if (camPos.x - follow.x < -camera.followDistance)
			{
				camPos.x += 5 * ((abs(camPos.x - follow.x) - camera.followDistance) / 30);
			}
			//Up
			if (camPos.y - follow.y > camera.followDistance)
			{
				camPos.y += -5 * ((abs(camPos.y - follow.y) - camera.followDistance) / 30);;
			}
			//Down
			if (camPos.y - follow.y < -camera.followDistance)
			{
				camPos.y += 5 * ((abs(camPos.y - follow.y) - camera.followDistance) / 30);;
			}

			//Set the camera view's center
			camera.cam.setCenter(camPos.x, camPos.y);
		}
	}

private:

};