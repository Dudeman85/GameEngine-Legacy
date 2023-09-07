#pragma once
#include <engine/Application.h>

extern ECS ecs;

using namespace engine;

struct Turret
{
	int health = 1;
	float projectileTimer = 0.2f;
	int shotsBeforeReload = 5;
	float reloadTime = 3;
	int fireSound = 0;
};

struct Projectile
{
	int damage = 1;
	bool destroy = false;
};

//Projectile controller system requires sprite, transform, rigidbody, box collider, Animator and Projectile
class ProjectileController : public System
{
public:
	ProjectileController()
	{
	};

	void Update()
	{
		for (auto const& entity : entities)
		{
			BoxCollider& collider = ecs.getComponent<BoxCollider>(entity);
			Projectile& projectile = ecs.getComponent<Projectile>(entity);
			Animator& animator = ecs.getComponent<Animator>(entity);
			Rigidbody& rb = ecs.getComponent<Rigidbody>(entity);
			Transform& tf = ecs.getComponent<Transform>(entity);

			if (collider.collisions.size() > 0 && !projectile.destroy)
			{
				if (collider.collisions.end() == find_if(collider.collisions.begin(), collider.collisions.end(), [projectile](const Collision& collision)
					{
						return collision.type == Collision::Type::tilemapTrigger || (ecs.hasComponent<Turret>(collision.a) || ecs.hasComponent<Turret>(collision.b) || (ecs.hasComponent<Projectile>(collision.a) && ecs.hasComponent<Projectile>(collision.b)));
					}))
				{
					projectile.destroy = true;
					rb.velocity = Vector2(0, 0);
					tf.xScale = 20;
					tf.yScale = 20;
					AnimationSystem::PlayAnimation(entity, "explosion");
					continue;
				}
			}

			if (projectile.destroy == true && !animator.playingAnimation)
			{
				ecs.destroyEntity(entity);
				break;
			}

			if (tf.x < -1000 || tf.y > 1000 || tf.x > 3000 || tf.y < -3000)
			{
				ecs.destroyEntity(entity);
				break;
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
		explosion = AnimationsFromSpritesheet("assets/explosion.png", 5, 1, vector<int>(5, 75))[0];
		defaultTexture = new Texture("assets/Gun_06.png");
		projectileTexture = new Texture("assets/bullet.png");

		//Register Projectile Controller
		ecs.registerComponent<Projectile>();
		projectileController = ecs.registerSystem<ProjectileController>();
		Signature projectileControllerSignature;
		projectileControllerSignature.set(ecs.getComponentId<Transform>());
		projectileControllerSignature.set(ecs.getComponentId<Projectile>());
		projectileControllerSignature.set(ecs.getComponentId<SpriteRenderer>());
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
						turret.fireSound = 1;
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
						turret.shotsBeforeReload = 5;
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
		ecs.addComponent(projectile, Transform{ .x = x, .y = y, .z = 5, .xScale = 7, .yScale = 7 });
		ecs.addComponent(projectile, SpriteRenderer{ .texture = projectileTexture });
		ecs.addComponent(projectile, Rigidbody{ .velocity = Vector2(targetTranform.x - x, targetTranform.y - y).Normalize() * speed, .kinematic = true });
		ecs.addComponent(projectile, BoxCollider{ .isTrigger = true });
		ecs.addComponent(projectile, Projectile{});
		ecs.addComponent(projectile, Animator{});

		AnimationSystem::AddAnimation(projectile, explosion, "explosion");
	}

	Entity CreateTurret(float x, float y)
	{
		Entity turret = ecs.newEntity();
		ecs.addComponent(turret, Transform{ .x = x, .y = y, .z = 1.5, .xScale = 60, .yScale = 20 });
		ecs.addComponent(turret, SpriteRenderer{ .texture = defaultTexture });
		ecs.addComponent(turret, Rigidbody{});
		ecs.addComponent(turret, BoxCollider{});
		ecs.addComponent(turret, Turret{});

		return turret;
	}

	Entity player;
	Animation explosion;
	shared_ptr<ProjectileController> projectileController;
	Texture* defaultTexture;
	Texture* projectileTexture;
};