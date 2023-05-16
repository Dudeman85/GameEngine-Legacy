#pragma once
#include <engine/Application.h>
#include <cmath>

extern ECS ecs;

using namespace engine;

//Player Component
struct Player
{
	bool jumpHeld = false;
	bool jumping = false;
	bool wallJumping = false;

	float maxSpeed = 2000;
	float accelSpeed = 400;
	float reactivityMult = 2;
	float maxJumpSpeed = 3800;
	float jumpAccelSpeed = 1000;

	// > 0 = yes
	float canWallJump = 0;
	int wallJumpDir = 0;
	float maxWallJumpSpeed = 4000;
	float wallJumpAccelSpeed = 2000;
};

//Player Controller requires Player, Sprite, Transform, BoxCollider, Rigidbody
class PlayerController : public System
{
public:
	PlayerController() {}

	void Update(GLFWwindow* window, double deltaTime, shared_ptr <PhysicsSystem> physicsSystem)
	{
		for (auto const& entity : entities)
		{
			Player& player = ecs.getComponent<Player>(entity);
			Rigidbody& rigidbody = ecs.getComponent<Rigidbody>(entity);
			BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);

			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				physicsSystem->Move(entity, Vector2(500, 0) * deltaTime);
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				physicsSystem->Move(entity, Vector2(-500, 0) * deltaTime);
			}
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				physicsSystem->Move(entity, Vector2(0, 500) * deltaTime);
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				physicsSystem->Move(entity, Vector2(0, -500) * deltaTime);
			}
		}
	}
};