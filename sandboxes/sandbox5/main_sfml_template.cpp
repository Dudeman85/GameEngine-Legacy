#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

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

    //Load the Vertex shader from file
    std::ifstream vertexShaderFile("VertexShader.glsl"); 
    std::string vertexShaderFileContents((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
    const char* vertexShaderSource = vertexShaderFileContents.c_str();

    //Load the Fragment shader form file
    std::ifstream fragmentShaderFile("FragmentShader.glsl"); 
    std::string fragmentShaderFileContents((std::istreambuf_iterator<char>(fragmentShaderFile)), std::istreambuf_iterator<char>());
    const char* fragmentShaderSource = fragmentShaderFileContents.c_str();

    //Create and compile the vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Check for error in compiling the vertex shader
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Fuck you, dumbass:\n" << infoLog << std::endl;
    }

    //Create and compile the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //Check for error in compiling the fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fuck you, dumbass:\n" << infoLog << std::endl;
    }

    //create shader program
    unsigned int shaderProgram = glCreateProgram();
    //OpenGL is kinda shit
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //Check for errors linking the shaders
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Fuck you, dumbass:\n" << infoLog << std::endl;
    }

    //Delete the shader programs after they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Use the shader program
    glUseProgram(shaderProgram);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

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


    glViewport(0, 0, 800, 600);

    //Set the resize window callback function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    //VBO is still bound to gl_array_buffer fo this works
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Main Loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        //Set the clear color to black and clear the screen
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}