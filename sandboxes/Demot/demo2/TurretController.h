#pragma once
#include <engine/Application.h>

extern ECS ecs;

using namespace engine;

struct Turret
{
	int health = 1;
	float projectileTimer = 0.2f;
	int shotsBeforeReload = 10;
	float reloadTime = 3;
};

struct Projectile
{
	int damage = 1;
};

//Pickup controller system requires sprite, transform, rigidbody, box collider and Projectile
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
				if (collider.collisions.end() == find_if(collider.collisions.begin(), collider.collisions.end(), [projectile](const Collision& collision)
					{
						return ecs.hasComponent<Turret>(collision.a) || ecs.hasComponent<Turret>(collision.b);
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
		defaultTexture = new Texture("assets/Gun_06.png");
		projectileTexture = new Texture("assets/bullet.png");

		//Register Projectile Controller
		ecs.registerComponent<Projectile>();
		projectileController = ecs.registerSystem<ProjectileController>();
		Signature projectileControllerSignature;
		projectileControllerSignature.set(ecs.getComponentId<Transform>());
		projectileControllerSignature.set(ecs.getComponentId<Projectile>());
		projectileControllerSignature.set(ecs.getComponentId<Sprite>());
		projectileControllerSignature.set(ecs.getComponentId<Rigidbody>());
		projectileControllerSignature.set(ecs.getComponentId<BoxCollider>());
		ecs.setSystemSignature<ProjectileController>(projectileControllerSignature);
	}

	void Update(double deltaTime)
	{
		projectileController->Update();

		for (auto const& entity : entities)
		{
			Turret& turret = ecs.getComponent<Turret>(entity);
			Transform& transform = ecs.getComponent<Transform>(entity);


			if (TransformSystem::Distance(entity, player) < 400)
			{
				transform.zRotation = TransformSystem::Angle(entity, player);
				if (turret.shotsBeforeReload > 0)
				{
					if (turret.projectileTimer <= 0)
					{
						SpawnProjectile(player, transform.x, transform.y, 5000);
						turret.projectileTimer = 0.2;
						turret.shotsBeforeReload--;
					}
					else
					{
						turret.projectileTimer -= deltaTime;
					}
				}
				else
				{
					if (turret.reloadTime <= 0)
					{
						turret.shotsBeforeReload = 10;
						turret.reloadTime = 3;
					}
					else
						turret.reloadTime -= deltaTime;
				}
			}
		}
	}

	void SpawnProjectile(Entity target, float x, float y, float speed)
	{
		Transform& targetTranform = ecs.getComponent<Transform>(target);

		Entity projectile = ecs.newEntity();
		ecs.addComponent(projectile, Transform{ .x = x, .y = y, .z = 1, .xScale = 10, .yScale = 10 });
		ecs.addComponent(projectile, Sprite{ .texture = projectileTexture });
		ecs.addComponent(projectile, Rigidbody{ .velocity = Vector2(targetTranform.x - x, targetTranform.y - y).Normalize() * speed, .kinematic = true });
		ecs.addComponent(projectile, BoxCollider{ .isTrigger = true });
		ecs.addComponent(projectile, Projectile{});
	}

	Entity CreateTurret(float x, float y)
	{
		Entity turret = ecs.newEntity();
		ecs.addComponent(turret, Transform{ .x = x, .y = y, .z = 1, .xScale = 40, .yScale = 20 });
		ecs.addComponent(turret, Sprite{ .texture = defaultTexture });
		ecs.addComponent(turret, Rigidbody{});
		ecs.addComponent(turret, BoxCollider{});
		ecs.addComponent(turret, Turret{});

		return turret;
	}

	Entity player;
	shared_ptr<ProjectileController> projectileController;
	Texture* defaultTexture;
	Texture* projectileTexture;
};