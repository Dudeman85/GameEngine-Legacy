#include <string>           // Inlude std::string

#include <kgfw/GLUtils.h>	// Include GLUtils for checkGLError
#include <glad/gl.h>		// Include glad
#include <GLFW/glfw3.h>		// Include glfw
#include <glm/glm.hpp>      // Include glm
#include <kgfw/Object.h>



void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
}  