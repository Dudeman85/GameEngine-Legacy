#pragma once
#include <chrono>

//ECS modules
#include "ECSCore.h"
#include "Sprite.h"
#include "Transform.h"
//#include "Gravity.h"
#include "Physics.h"

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
		shared_ptr<AnimationSystem> animationSystem;
		shared_ptr<RenderSystem> renderSystem;
		shared_ptr<TransformSystem> transformSystem;
		shared_ptr<PhysicsSystem> physicsSystem;

		EngineLib()
		{
			//Make sure OpenGL context has been created
			assert(OPENGL_INITIALIZED && "OpenGL has not been initialized! Create a window, or manually create the OpenGL context before initializing EngineLib!");

			//Init time
			lastFrame = chrono::high_resolution_clock::now();

			//Register all default engine components here
			ecs.registerComponent<Sprite>();
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

			//Render System
			renderSystem = ecs.registerSystem<RenderSystem>();
			Signature renderSystemSignature;
			renderSystemSignature.set(ecs.getComponentId<Sprite>());
			renderSystemSignature.set(ecs.getComponentId<Transform>());
			ecs.setSystemSignature<RenderSystem>(renderSystemSignature);

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
			renderSystem->Update(cam);

			//Calculate Delta Time
			chrono::time_point thisFrame = chrono::high_resolution_clock::now();
			chrono::duration<double> duration = thisFrame - lastFrame;
			deltaTime = duration.count();
			lastFrame = thisFrame;

			return deltaTime;
		}
	
	private:
		chrono::time_point<chrono::high_resolution_clock> lastFrame;
	};
}