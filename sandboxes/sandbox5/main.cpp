#include <engine/Application.h>




using namespace engine;

ECS ecs;

void main(){

	GLFWwindow * window = engine::CreateWindow(800, 600, "Windos");

 engine::EngineLib engine;

 engine::Camera cam = engine::Camera(800, 600);


 engine::RenderSystem::SetBackgroundColor(0, 0, 0); // tämä toimii decimali rgb codi 


 Texture textura("assets/strawberry.png", GL_LINEAR);
 Texture textura1("assets/laatikko.png", GL_LINEAR);
 Texture textura2("assets/latikko.png", GL_LINEAR);
 Texture textura3("assets/lentoKone.png", GL_LINEAR);



 // Tee uusi Entiteetti. crea una nueva entida 
 Entity sprite = ecs.newEntity();
 
 // Add the sprite component with a texture and default shader
 // agregae el componente sprite con una textura y un shader predecterminado
 ecs.addComponent(sprite, Sprite{ &textura });

 //Add the transform component which is required for the Render System
 // Agregar el componente de trasformacion, que es nesesario para el Render System
 // dentro de clase Transform estan los componetes relacionados con la (rotacion , escala y posicion)
 ecs.addComponent(sprite, Transform{ .xScale  = 10, .yScale = 10 });

 

 while (!glfwWindowShouldClose(window))
 {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		 glfwSetWindowShouldClose(window, true);


	 engine.Update(&cam);



	 glfwSwapBuffers(window);
	 glfwPollEvents();

 }



 glfwTerminate();



};

