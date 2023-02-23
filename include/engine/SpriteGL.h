#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <engine/ECSCore.h>
#include <engine/Transform.h>
#include <engine/GL/Shader.h>
#include <engine/GL/Texture.h>

namespace engine
{
	//Sprite component
	struct Sprite
	{
		Texture texture;
		Shader* shader = nullptr;
	};

	//Render system
	class RenderSystem : public System
	{
		void Update()
		{

		}
	};
}