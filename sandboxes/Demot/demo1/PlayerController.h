#pragma once
#include "engine/Application.h"

using namespace engine;

//player component
struct Player 
{
	float moveSpeed = 15.f;
	float jumpSpeed = 30.f;
	int moveState;
	

};
//Player control system
class PlayerControl : public System
{
public:
	

	void Update()
	{
		for (auto const& entity : entities)
		{
			Player& player = ecs.getComponent<Player>(entity);
			Animator& anim = ecs.getComponent<Animator>(entity);
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				lib->physicsSystem->Velocity(entity, player.moveSpeed,0,1);
				if (anim.currentAnimation != "Left")
					lib->animationSystem->PlayAnimation(entity, "Left");
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				lib->physicsSystem->Velocity(entity, player.moveSpeed, 0, 3);
				if (anim.currentAnimation != "Right")
					lib->animationSystem->PlayAnimation(entity, "Right");
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				lib->physicsSystem->Velocity(entity, 0, player.jumpSpeed, 4);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				lib->physicsSystem->Velocity(entity, player.moveSpeed, 0, 2);
			}
		}

	}






	EngineLib* lib;

};