#pragma once
#include <engine/Application.h>

extern ECS ecs;

using namespace engine;

struct Turret
{
	int health = 1;
	float projectileTimer = 2;
	int magicSound = 0;
};

struct Projectile
{
	int damage = 1;
};

//Pickup controller system requires sprite, transform, rigidbody, box collider, animator and Projectile
class ProjectileController : public System
{
public:
	ProjectileController() {};

	void Update()
	{
		for (auto const& entity : entities)
		{
			BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);
			Projectile& projectile = ecs.getComponent<Projectile>(entity);
			
			if (collider.collisions.size() > 0)
			{
				if (collider.collisions.end() == find_if(collider.collisions.begin(), collider.collisions.end(), [](const Collision& collision)
					{
						return (ecs.hasComponent<Turret>(collision.a) || ecs.hasComponent<Turret>(collision.b)) && collision.type != Collision::Type::tilemapTrigger;
					}))
				{
					ecs.destroyEntity(entity);
					break;
				}
			}
		}
	}
};

//Pickup controller system requires sprite, transform, rigidbody, box collider, animator and Turret
class TurretController : public System
{
public:
	TurretController()
	{
		animations = AnimationsFromSpritesheet("assets/mageattack.png", 4, 2, vector<int>{250, 250, 250, 250, 150, 150, 150, 150});
		projectileAnim = AnimationsFromSpritesheet("assets/magebullet.png", 5, 1, vector<int>(5 * 1, 50))[0];

		//Register Projectile Controller
		ecs.registerComponent<Projectile>();
		projectileController = ecs.registerSystem<ProjectileController>();
		Signature projectileControllerSignature;
		projectileControllerSignature.set(ecs.getComponentId<Transform>());
		projectileControllerSignature.set(ecs.getComponentId<Projectile>());
		projectileControllerSignature.set(ecs.getComponentId<Sprite>());
		projectileControllerSignature.set(ecs.getComponentId<Rigidbody>());
		projectileControllerSignature.set(ecs.getComponentId<BoxCollider>());
		projectileControllerSignature.set(ecs.getComponentId<Animator>());
		ecs.setSystemSignature<ProjectileController>(projectileControllerSignature);
	}

	void Update(double deltaTime)
	{
		projectileController->Update();

		for (auto const& entity : entities)
		{
			Turret& turret = ecs.getComponent<Turret>(entity);
			Transform& transform = ecs.getComponent<Transform>(entity);
			Animator& animator = ecs.getComponent<Animator>(entity);
			Entity playerAttack = ecs.getComponent<Player>(player).attackHitbox;

			//Only active when player is near
			if (TransformSystem::Distance(entity, player) < 400)
			{
				//When hit by player play dying animation
				if (PhysicsSystem::AABBIntersect(entity, playerAttack).type != Collision::Type::miss && animator.playingAnimation)
				{
					AnimationSystem::PlayAnimation(entity, "die");
					turret.projectileTimer = 999;
				}

				//When done dying
				if (!animator.playingAnimation)
				{
					ecs.destroyEntity(entity);
					break;
				}

				if (turret.projectileTimer <= 0)
				{
					SpawnProjectile(player, transform.x, transform.y, 2000);
					turret.projectileTimer = 2;
					turret.magicSound = 1;
				}
			}
			turret.projectileTimer -= deltaTime;
		}
	}

	void SpawnProjectile(Entity target, float x, float y, float speed)
	{
		Transform& targetTranform = ecs.getComponent<Transform>(target);

		Entity projectile = ecs.newEntity();
		ecs.addComponent(projectile, Transform{ .x = x, .y = y, .z = 1.5, .xScale = 20, .yScale = 20 });
		ecs.addComponent(projectile, Sprite{});
		ecs.addComponent(projectile, Rigidbody{ .velocity = Vector2(targetTranform.x - x, targetTranform.y - y).Normalize() * speed, .kinematic = true });
		ecs.addComponent(projectile, BoxCollider{ .isTrigger = true });
		ecs.addComponent(projectile, Animator{});
		ecs.addComponent(projectile, Projectile{});
		AnimationSystem::AddAnimation(projectile, projectileAnim, "default");
		AnimationSystem::PlayAnimation(projectile, "default", true);
	}

	Entity CreateTurret(float x, float y)
	{
		Entity turret = ecs.newEntity();
		ecs.addComponent(turret, Transform{ .x = x, .y = y, .z = 0, .xScale = 28, .yScale = 37 });
		ecs.addComponent(turret, Sprite{});
		ecs.addComponent(turret, Rigidbody{});
		ecs.addComponent(turret, BoxCollider{ .scale = Vector2(0.9, 1), .offset = Vector2(0, 2) });
		ecs.addComponent(turret, Animator{});
		ecs.addComponent(turret, Turret{});
		AnimationSystem::AddAnimations(turret, animations, vector<string>{"default", "die"});
		AnimationSystem::PlayAnimation(turret, "default", true);

		return turret;
	}

	Entity player;
	shared_ptr<ProjectileController> projectileController;
	vector<Animation> animations;
	Animation projectileAnim;
};