#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <engine/GL/Shader.h>

namespace engine
{
	//Sprite component
	struct Sprite
	{
		
		Shader shader = Shader();
	};

	//Sprite system
	class SpriteSystem
	{
		void Update()
		{

		}
	};
}