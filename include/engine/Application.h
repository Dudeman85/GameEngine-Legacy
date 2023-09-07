#pragma once
#include <chrono>

//ECS modules
#include <engine/ECSCore.h>	
#include <engine/Sprite.h>
#include <engine/Transform.h>
#include <engine/Physics.h>
#include <engine/ModelSystem.h>

//Other engine libs
#include <engine/GL/Window.h>
#include <engine/Image.h>
#include <engine/AL/SoundDevice.h>
#include <engine/AL/SoundSource.h>
#include <engine/AL/SoundBuffer.h>
#include <engine/AL/MusicBuffer.h>
using namespace std;

extern ECS ecs;

namespace engine 
{
	//A class to store all the default engine modules
	class EngineLib
	{
	public:
		SoundDevice* soundDevice;
		double deltaTime = 0;
		double programTime = 0;

		shared_ptr<TransformSystem> transformSystem;
		shared_ptr<RenderSystem> spriteRenderSystem;
		shared_ptr<ModelRenderSystem> modelRenderSystem;
		shared_ptr<AnimationSystem> animationSystem;
		shared_ptr<PhysicsSystem> physicsSystem;

		EngineLib()
		{
			//Make sure OpenGL context has been created
			assert(OPENGL_INITIALIZED && "OpenGL has not been initialized! Create a window, or manually create the OpenGL context before initializing EngineLib!");

			//Init time
			lastFrame = chrono::high_resolution_clock::now();

			//Register all default engine components here
			ecs.registerComponent<Sprite>();
			ecs.registerComponent<ModelRenderer>();
			ecs.registerComponent<Transform>();
			ecs.registerComponent<Animator>();
			ecs.registerComponent<Rigidbody>();
			ecs.registerComponent<BoxCollider>();

			//Register all default engine systems here
			//Transform System
			transformSystem = ecs.registerSystem<TransformSystem>();
			Signature transformSystemSignature;
			transformSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<TransformSystem>(transformSystemSignature);

			//Sprite Render System
			spriteRenderSystem = ecs.registerSystem<RenderSystem>();
			Signature spriteRenderSystemSignature;
			spriteRenderSystemSignature.set(ecs.getComponentId<Sprite>());
			spriteRenderSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<RenderSystem>(spriteRenderSystemSignature);

			//Model Render System
			modelRenderSystem = ecs.registerSystem<ModelRenderSystem>();
			Signature modelRenderSystemSignature;
			modelRenderSystemSignature.set(ecs.getComponentId<ModelRenderer>());
			modelRenderSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<ModelRenderSystem>(modelRenderSystemSignature);

			//Animation System
			animationSystem = ecs.registerSystem<AnimationSystem>();
			Signature animationSystemSignature;
			animationSystemSignature.set(ecs.getComponentId<Sprite>());
			animationSystemSignature.set(ecs.getComponentId<Animator>());
			ecs.setSystemSignature<AnimationSystem>(animationSystemSignature);

			//Physics System
			physicsSystem = ecs.registerSystem<PhysicsSystem>();
			Signature physicsSystemSignature;
			physicsSystemSignature.set(ecs.getComponentId<Rigidbody>());
			physicsSystemSignature.set(ecs.getComponentId<Transform>());
			physicsSystemSignature.set(ecs.getComponentId<BoxCollider>());
			ecs.setSystemSignature<PhysicsSystem>(physicsSystemSignature);

			soundDevice = SoundDevice::getDevice()->getDevice();
		}

		//Updates all default engine systems, calculates and returns delta time
		double Update(Camera* cam)
		{
			//Update engine systems
			transformSystem->Update();
			physicsSystem->Update(deltaTime);
			animationSystem->Update(deltaTime);
			spriteRenderSystem->Update(cam);
			modelRenderSystem->Update(cam);

			//Calculate Delta Time
			chrono::time_point thisFrame = chrono::high_resolution_clock::now();
			chrono::duration<double> duration = thisFrame - lastFrame;
			deltaTime = duration.count();
			lastFrame = thisFrame;
			programTime += deltaTime;

			return deltaTime;
		}
	
	private:
		chrono::time_point<chrono::high_resolution_clock> lastFrame;
	};
}