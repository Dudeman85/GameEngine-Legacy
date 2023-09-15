#include <engine/Application.h>
#include <engine/TextRender.h>

using namespace std;
using namespace engine;

ECS ecs;



int main()
{
	// Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = CreateWindow(800, 600, "Text rendering");
	
	// Camera
	Camera cam = Camera(800, 600);
	cam.SetPosition(0, 0, 10);

	// Initialize the default engine library
	EngineLib engine;
	
	// Window background colour
	SpriteRenderSystem::SetBackgroundColor(0, 120, 0);


	// Text
	TextRender text;
	//vector<TrueFont> trueFont0 = text.SetUpTTF("assets/fonts/ARIAL.TTF", 0, 0, 48);
	//vector<TrueFont> trueFont1 = text.SetUpTTF("assets/fonts/COMIC.TTF", 0, 0, 48);
	FT_Face face0;
	//map<FT_Face, TrueFont> Faces = text.SetUpTTF(face0, "assets/fonts/ARIAL.TTF", 0, 0, 48);
	//text.LoadText(Faces);





	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		// Update all engine systems, this usually should go last in the game loop
		// For greater control of system execution, you can update each one manually
		engine.Update(&cam);

		//text.RenderText(trueFont0, &cam, "This is sample text", -390.0f, -290.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		//text.RenderText(trueFont1, &cam,"(C) LearnOpenGL.com", 150.0f, 275.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		// OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}