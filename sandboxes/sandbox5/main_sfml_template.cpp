#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include <engine/GL/Shader.h>

//Resize window callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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
		-0.5f,  0.5f, 0.0f, //Top Left
		0.5f, 0.5f, 0.0f, //Top Right
		0.5f, -0.5f, 0.0f, //Bottom Right
		-0.5f, -0.5f, 0.0f, //Bottom Left
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

	//This configures the vertex attribute at location 0 aka the vertex posistion. It takes its data from VBO since it was just bound
	//layout (location = 0) in vertex shader, so first parameter is the vertex attribute in the shader, 
	//second parameter is the size of the vertex attribute, so 3 for vec3
	//vec3 is float so third parameter is the type
	//Fourth is normalize integer data, we dont want this
	//Fifth is the amount of bytes between the consecutive attributes
	//Sixth is the offset of the data in the buffer, should be 0 so cast 0 to void*
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Enable attribute at location 0 aka position
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//Main Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//Set the clear color to black and clear the screen
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shader.ID, "color");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}