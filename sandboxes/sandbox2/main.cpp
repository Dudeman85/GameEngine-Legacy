

#include <engine/Application.h>
#include <engine/TextRender.h>
//#include <enet/enet.h>

// enet wrappaus ja cpp:stä enetin kutsu tai CreateWindows funktion nimen muutos enginessä
#if defined(_WIN32)
	#define WIN32_LEAN_AND_MEAN
#endif
#include <enet/enet.h>


using namespace std;
using namespace engine;

ECS ecs;



int main()
{
	// Create the window and OpenGL context before creating EngineLib
	GLFWwindow* window = engine::CreateGLWindow(800, 600, "Text rendering");
	
	// Camera
	Camera cam = Camera(800, 600);
	cam.SetPosition(0, 0, 10);

	// Initialize the default engine library
	EngineLib engine;
	
	// Window background colour
	SpriteRenderSystem::SetBackgroundColor(0, 120, 0);


	
	//Font arialFont("assets/fonts/ARIAL.TTF");

	// Text
	TextRender text0;
	FT_Face face0 = 0;
	const char* font0 = "Arial";
	vector<FT_Face> Faces0 = text0.SetUpTTF(face0, "assets/fonts/ARIAL.TTF", 0, 0, 48);
	text0.LoadText(Faces0);




	TextRender text1;
	FT_Face face1 = 0;
	const char* font1 = "Wingdings";
	vector<FT_Face> Faces1 = text1.SetUpTTF(face1, "assets/fonts/wingding.ttf", 0, 0, 48);
	text1.LoadText(Faces1);





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

		text0.RenderText(font0, &cam, "This1", -390.0f, -290.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		//arialText.render(0,0,"fdfds");
		///arialText.render(10,0,"fdsfsfdsf");

		text1.RenderText(font1, &cam, "This2", 150.0f, 275.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		//cout << Faces[0]->glyph->bitmap << endl;

		// OpenGL stuff, goes very last
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}