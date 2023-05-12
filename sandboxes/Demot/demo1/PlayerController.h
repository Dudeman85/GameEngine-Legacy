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

	void Update(GLFWwindow* window, double deltaTime)
	{
		for (auto const& entity : entities)
		{
			Player& player = ecs.getComponent<Player>(entity);
			Rigidbody& rigidbody = ecs.getComponent<Rigidbody>(entity);
			BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);

			//Movement
			//Right
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				if (rigidbody.velocity.x < player.maxSpeed)
				{
					Vector2 impulse;

					//Decelerate faster
					if (rigidbody.velocity.x < 0)
						impulse = Vector2(min(player.accelSpeed * player.reactivityMult, player.maxSpeed - rigidbody.velocity.x), 0);
					else
						impulse = Vector2(min(player.accelSpeed, player.maxSpeed - rigidbody.velocity.x), 0);

					PhysicsSystem::Impulse(entity, impulse);
				}
			}
			//Left
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				if (rigidbody.velocity.x > -player.maxSpeed)
				{
					Vector2 impulse;

					//Decelerate faster
					if (rigidbody.velocity.x > 0)
						impulse = Vector2(max(-player.accelSpeed * player.reactivityMult, -player.maxSpeed - rigidbody.velocity.x), 0);
					else
						impulse = Vector2(max(-player.accelSpeed, -player.maxSpeed - rigidbody.velocity.x), 0);

					PhysicsSystem::Impulse(entity, impulse);
				}
			}
			//No direction
			else
			{
				//Slow the player down aka friction
				if (abs(rigidbody.velocity.x) > 0)
				{
					//If touching ground decelerate faster
					rigidbody.velocity.x -= (rigidbody.velocity.x / abs(rigidbody.velocity.x) * min(player.accelSpeed, abs(rigidbody.velocity.x)));
				}
			}

			//Set can wall jump for 100ms after touching wall
			if (collider.sidesCollided[Direction::left] || collider.sidesCollided[Direction::right])
			{
				//Enable Walljump
				if (!player.jumpHeld && !collider.sidesCollided[Direction::down])
				{
					//Right wall
					if (collider.sidesCollided[Direction::right])
					{
						player.canWallJump = 0.1;
						player.wallJumpDir = -1;
					}

					//Left wall
					if (collider.sidesCollided[Direction::left])
					{
						player.canWallJump = 0.1;
						player.wallJumpDir = 1;
					}
				}

				//Wall Slide
				rigidbody.velocity.y = max(rigidbody.velocity.y, -1000.f);
			}

			//When jump is pressed
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				//If touching ground
				if (collider.sidesCollided[Direction::down])
				{
					player.jumping = true;
				}
				else
				{
					if (player.canWallJump > 0)
						player.wallJumping = true;
				}

				//Accelerate to max jump speed while holding jump
				if (player.jumping)
				{
					//If ceiling is hit cancel the jump
					if (collider.sidesCollided[Direction::up])
						player.jumping = false;

					if (rigidbody.velocity.y + player.jumpAccelSpeed < player.maxJumpSpeed)
					{
						PhysicsSystem::Impulse(entity, Vector2(0, player.jumpAccelSpeed));
					}
					else //Once max speed has been reached
					{
						rigidbody.velocity.y = player.maxJumpSpeed;
						player.jumping = false;
					}
				}

				//Accelerate to max wall jump speed while holding jump
				if (player.wallJumping)
				{
					player.canWallJump = 0;

					//If ceiling is hit cancel the jump
					if (collider.sidesCollided[Direction::up])
						player.wallJumping = false;

					if (rigidbody.velocity.y + player.wallJumpAccelSpeed < player.maxWallJumpSpeed)
					{
						PhysicsSystem::Impulse(entity, Vector2(player.wallJumpDir * player.wallJumpAccelSpeed / 2, player.wallJumpAccelSpeed));
					}
					else //Once max speed has been reached
					{
						rigidbody.velocity = Vector2(player.wallJumpDir * player.maxWallJumpSpeed / 2, player.maxWallJumpSpeed);
						player.wallJumping = false;
					}
				}

				player.jumpHeld = true;
			}
			else
			{
				player.wallJumping = false;
				player.wallJumpDir = 0;
				player.jumping = false;
				if (player.jumpHeld)
				{
					//If not touching ground smooth the analog jump apex
					if (!collider.sidesCollided[Direction::down])
					{
						rigidbody.velocity = Vector2(rigidbody.velocity.x, min(rigidbody.velocity.y, 1500.0f));
					}
					player.jumpHeld = false;
				}
			}

			if (player.canWallJump > 0)
				player.canWallJump -= deltaTime;
		}
	}
};