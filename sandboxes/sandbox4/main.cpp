#include <engine/Application.h>
#include <engine/GL/Model.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <engine/ModelSystem.h>

using namespace engine;

//Creates a global instance of the ECS manager
ECS ecs;

int main()
{
	//Create the window and OpenGL context before creating EngineLib
	//Paraeters define window size(x,y) and name
	GLFWwindow* window = CreateWindow(800, 600, "Window");
	//Initialize the default engine library
	EngineLib engine;
	//Create the camera
	Camera cam = Camera(800, 600);
	cam.perspective = false;
	cam.SetPosition(0, 0, 500);

	//changes window backround color
	RenderSystem::SetBackgroundColor(0, 120, 0);


	//MODEL SYSTEM TESTING
	//Register Model Renderer
	ecs.registerComponent<ModelRenderer>();
	shared_ptr<ModelRenderSystem> modelRender = ecs.registerSystem<ModelRenderSystem>();
	Signature modelRenderSystemSignature;
	modelRenderSystemSignature.set(ecs.getComponentId<Transform>());
	modelRenderSystemSignature.set(ecs.getComponentId<ModelRenderer>());
	ecs.setSystemSignature<ModelRenderSystem>(modelRenderSystemSignature);

	//MODEL TESTING
	Shader s(R"(
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

	/*
	Entity sprite = ecs.newEntity();
	Texture texture("assets/strawberry.png");
	ecs.addComponent(sprite, Transform{.xScale = 1, .yScale = 1});
	ecs.addComponent(sprite, Sprite{ .texture = &texture });
	*/
	Model model("assets/suzanne.obj");

	Entity suzanne = ecs.newEntity();
	Transform& suzanneTransform = ecs.addComponent(suzanne, Transform{ .x = 0, .xScale = 10, .yScale = 10, .yRotation = 0 });
	ecs.addComponent(suzanne, ModelRenderer{ .model = &model });

	Entity suzanne2 = ecs.newEntity();
	Transform& suzanne2Transform = ecs.addComponent(suzanne2, Transform{ .x = 100, .y = -100, .xScale = 7, .yScale = 7, .yRotation = 0 });
	ecs.addComponent(suzanne2, ModelRenderer{ .model = &model });

	//Game Loop
	while (!glfwWindowShouldClose(window))
	{
		//Close window when Esc is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		//Update all engine systems, this usually should go last in the game loop
		//For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			suzanneTransform.yRotation += -1;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			suzanneTransform.yRotation += 1;

		suzanne2Transform.zRotation += 1;
		suzanne2Transform.yRotation += 1;

		modelRender->Update(&cam);

		//OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}