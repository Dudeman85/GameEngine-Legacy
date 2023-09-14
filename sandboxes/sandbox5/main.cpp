#include <engine/Application.h>




using namespace engine;

ECS ecs;



int main()
{

	GLFWwindow* window = engine::CreateWindow(800, 600, "Windos");

	engine::EngineLib engine;

	engine::Camera cam = engine::Camera(800, 600);
	cam.SetPosition(0, 0, 1000);


	engine::SpriteRenderSystem::SetBackgroundColor(0, 0, 120); // tämä toimii decimali rgb codi 

	Model model("assets/Achelous.obj");

	/*Texture textura1("assets/laatikko.png", GL_LINEAR);
	Texture textura2("assets/latikko.png", GL_LINEAR);*/

	// Tee uusi Entiteetti. crea una nueva entida 

	Entity zuzanne = ecs.newEntity();
	/*Entity sprite1 = ecs.newEntity();*/


	// Add the sprite component with a texture and default shader
	// agregae el componente sprite con una textura y un shader predecterminado

	Transform& suzanneTransform = ecs.addComponent(zuzanne, Transform{ .position = Vector3(0,0,0), .rotation = Vector3(0,0,0), .scale = Vector3(30)  });

	//Add the transform component which is required for the Render System
	// Agregar el componente de trasformacion, que es nesesario para el Render System
	// dentro de clase Transform estan los componetes relacionados con la (rotacion , escala y posicion)

	ecs.addComponent(zuzanne, ModelRenderer{ .model = &model });
	/*ecs.addComponent(sprite1, Transform{ .x = 10, .y = -100, .xScale = 100, .yScale = 100 });
	ecs.addComponent(sprite2, Transform{ .x = -100, .y = 100, .xScale = 100, .yScale = 100 });*/




	engine.modelRenderSystem->defaultShader = new Shader(
		R"( //////////////// vexterShader ////////////////////////////
				#version 330 core
				layout(location = 0) in vec3 aPos;
				layout(location = 1) in vec3 aNormal;
				layout(location = 2) in vec2 aTexCoords;
                
				
                out vec3 FragPos;
                out vec3 Normal;
                out vec2 TexCoords;


				uniform mat4 model;
				uniform mat4 view;
				uniform mat4 projection;

				void main()
				{
					TexCoords = aTexCoords;
                    FragPos = vec3 (model * vec4(aPos,1.0));
                    Normal = aNormal;
					gl_Position = projection * view * model * vec4(aPos, 1.0);
                    
				}
				)",
		R"( ////////////////// FragmerSheder///////////////////////////////////////////////
				#version 330 core
				out vec4 FragColor;
 

				in vec2 TexCoords;
                in vec3 Normal;
                in vec3 FragPos;

                uniform sampler2D texture_diffuse1;
				uniform vec3 lightColor;
				uniform vec3 lightPos;
//////////////////////////uniform vec3 objectColor;////////////////// now we don't need this declaration 
				void main()
				{    
					vec3 objectColor = texture(texture_diffuse1, TexCoords).xyz;
//////////////////////////////////// Ambient //////////////////////////////////////////////////////////
					float ambientStrength = 0.1;
					vec3 ambient = ambientStrength * lightColor;
                    
///////////////////////////////// diffuce //////////////////////////////////////////////////////////////
                    vec3 norm = normalize(Normal);
                    vec3 lightDir = normalize(lightPos - FragPos);
                    float diff = max(dot(norm , lightDir), 0.0);
                    vec3 diffuse = diff * lightColor;

                   vec3 result = (ambient + diffuse) * objectColor;
                   FragColor = vec4(result, 1.0);  
				   //FragColor = vec4(ambient, 1.0) * objectColor;         
				}
				)", false);




	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			suzanneTransform.rotation.y += 1;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			suzanneTransform.rotation.y += -1;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			suzanneTransform.rotation.x += -1;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			suzanneTransform.rotation.x += 1;



		engine.Update(&cam);
		/*suzanneTransform.rotation = +1;
		suzanneTransform.rotation = +1;*/




		glfwSwapBuffers(window);
		glfwPollEvents();

	}



	glfwTerminate();


	return 0;
};

