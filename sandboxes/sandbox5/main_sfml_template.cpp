#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <cmath>

#include <engine/GL/Shader.h>
#include <engine/GL/Camera.h>
#include <engine/GL/Texture.h>

using namespace engine;

Camera cam = Camera(800, 600);

//Resize window callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cam.Translate(1, 0);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cam.Translate(-1, 0);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cam.Translate(0, 1);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cam.Translate(0, -1);

	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		cam.Translate(0, 0, -1);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		cam.Translate(0, 0, 1);
}

int main()
{
	//Initialize GLFW and set it to require OpenGL 3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Set the resize window callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glViewport(0, 0, 800, 600);

	//Load the shaders from files and use the compiled shader program
	Shader shader("VertexShader.glsl", "FragmentShader.glsl");
	shader.use();
	
	//Rectangle vertices start at top left and go clockwise to bottom left
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.f,  1.f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.f, -1.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.f, -1.f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.f,  1.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	float textureCoords[]{
		0, 1, //Top Left
		1, 1, //Top Right
		1, 0, //Bottom Right
		0, 0, //Bottom Left
	};
	//Indices to draw a rectangle
	unsigned int indices[] = {
		0, 1, 2, //1st trangle
		0, 2, 3, //2nd triangle
	};

	//Make the Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind the Vertex Array Object
	glBindVertexArray(VAO);

	//Bind the Vertex Bufer Object and set vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind and set indices to EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Configure Vertex attribute at location 0 aka position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Configure Vertex attribute at location 1 aka color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Configure Vertex attribute at location 2 aka texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	Texture texture = Texture("strawberry.png", GL_NEAREST);

	//Main Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//Set the clear color to black and clear the screen
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.f, 0.f, -10.f));
		trans = glm::scale(trans, glm::vec3(100.5, 100.5, 1));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

		unsigned int transformLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		unsigned int projLoc = glGetUniformLocation(shader.ID, "projection");
		glm::mat4 a = cam.GetProjectionMatrix();
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(a));

		unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.GetViewMatrix()));

		glBindTexture(GL_TEXTURE_2D, texture.GetID());
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}