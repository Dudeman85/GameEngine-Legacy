# Getting Started



## Downloading game-engine

Engine can bo found at https://repo.kamit.fi/ttv21sp_pelimoottoriohjelmointi/tiimi4

You can download suorce files in archive or clone with git.

You need to compile the project with CMake before you can use the engine.
First add filepath to source, and then define path to build folder, for example C:/....filepath.../tiimi4.
It is recommended to create build folder into your source folder, for example C:/....filepath.../tiimi4/build
After defining filepaths, you can first configure the project. CMake asks you to specify the generator for the project. select visual studio compiler you are using. Leave other fields empty or default.
Once configuration is completed, generate build files and you can open project.



## Creating window and game loop

Create new .cpp file for your game and add following lines at the beginning:
```cpp
#include <engine/Application.h>

using namespace std;
using namespace engine;
```

In your main function:
```cpp
//Create the window and OpenGL context before creating EngineLib
//Values define window size(x,y) and name
GLFWwindow* window = CreateWindow(800, 600, "Window");
//Initialize the default engine library
EngineLib engine;
//Create the camera
Camera cam = Camera(800, 600);

//changes window backround color, values 0-1, RGB
RenderSystem::SetBackgroundColor(0, .5, .1);
```

Once window is created, you can create game loop

```cpp
//Game Loop
while (!glfwWindowShouldClose(window))
{
	//
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
		
   
    //Update all engine systems, this usually should go last in the game loop
	//For greater control of system execution, you can update each one manually
	engine.Update(&cam);

	//OpenGL stuff, goes very last
	glfwSwapBuffers(window);
	glfwPollEvents();
}
glfwTerminate();

```


```cpp
koodilaatikon malli
```