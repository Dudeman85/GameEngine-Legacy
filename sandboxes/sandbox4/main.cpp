#include <engine/Application.h>
#include <engine/GL/Model.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Creates a global instance of the ECS manager
ECS ecs;

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	//Paraeters define window size(x,y) and name
	GLFWwindow* window = engine::CreateWindow(800, 600, "Window");
	//Initialize the default engine library
	engine::EngineLib engine;
	//Create the camera
	engine::Camera cam = engine::Camera(800, 600);
	cam.SetPosition(0, 0, -90);


	//changes window backround color
	engine::RenderSystem::SetBackgroundColor(0, 120, 0);


	//MODEL TESTING
	engine::Model suzanne("assets/suzanne.obj");
	engine::Shader s(R"(
		#version 330 core
		layout(location = 0) in vec3 aPos;
		layout(location = 1) in vec3 aNormal;
		layout(location = 2) in vec2 aTexCoords;

		out vec2 TexCoords;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			TexCoords = aTexCoords;
			gl_Position = projection * view * model * vec4(aPos, 1.0);
		}
		)",
		R"(
		#version 330 core
		out vec4 FragColor;

		in vec2 TexCoords;

		uniform sampler2D texture_diffuse1;

		void main()
		{    
			FragColor = texture(texture_diffuse1, TexCoords);
		}
		)", false);


	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//Close window when Esc is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		suzanne.Draw(&s);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}