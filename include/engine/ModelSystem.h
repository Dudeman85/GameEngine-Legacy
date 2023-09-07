#pragma once
#include <engine/ECSCore.h>
#include <engine/Transform.h>
#include <engine/GL/Shader.h>
#include <engine/GL/Texture.h>
#include <engine/GL/Camera.h>

extern ECS ecs;

namespace engine
{
	//3D Model component
	struct Model
	{

	};

	//3D Model Render System, requires Transform and Model
	class ModelRenderSystem : public System
	{
	public:
		ModelRenderSystem();


	private:

	};
}