#include <engine/Application.h>




using namespace engine;

ECS ecs;



int main()
{

	GLFWwindow* window = engine::CreateWindow(800, 600, "Windos");

	engine::EngineLib engine;

	engine::Camera cam = engine::Camera(800, 600);
	cam.SetPosition(0, 0, 500);


	engine::SpriteRenderSystem::SetBackgroundColor(0, 0, 120); // tämä toimii decimali rgb codi 

	Model model("assets/suzanne.obj");

	/*Texture textura1("assets/laatikko.png", GL_LINEAR);
	Texture textura2("assets/latikko.png", GL_LINEAR);*/

	// Tee uusi Entiteetti. crea una nueva entida 

	Entity zuzanne = ecs.newEntity();
	/*Entity sprite1 = ecs.newEntity();*/


	// Add the sprite component with a texture and default shader
	// agregae el componente sprite con una textura y un shader predecterminado

	Transform& suzanneTransform = ecs.addComponent(zuzanne, Transform{ .position = Vector3(0,0,0), .rotation = Vector3(0,0,0), .scale = Vector3(100, 100, 100)  });

	//Add the transform component which is required for the Render System
	// Agregar el componente de trasformacion, que es nesesario para el Render System
	// dentro de clase Transform estan los componetes relacionados con la (rotacion , escala y posicion)

	ecs.addComponent(zuzanne, ModelRenderer{ .model = &model });
	/*ecs.addComponent(sprite1, Transform{ .x = 10, .y = -100, .xScale = 100, .yScale = 100 });
	ecs.addComponent(sprite2, Transform{ .x = -100, .y = 100, .xScale = 100, .yScale = 100 });*/

	engine.modelRenderSystem->defaultShader = new Shader(
		R"(
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
				uniform vec3 lightColor;
				void main()
				{    
					vec4 objectColor = texture(texture_diffuse1, TexCoords);
					float ambientStrength = 0.3;
					vec3 ambient = ambientStrength * lightColor;

					FragColor = vec4(ambient, 1.0) * objectColor;
				}
				)", false);




	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			suzanneTransform.rotation.y += -1;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			suzanneTransform.rotation.y += 1;


		engine.Update(&cam);
	/*	suzanneTransform.zRotation = +1;
		suzanneTransform.yRotation = +1;*/




		glfwSwapBuffers(window);
		glfwPollEvents();

	}



	glfwTerminate();


	return 0;
};

