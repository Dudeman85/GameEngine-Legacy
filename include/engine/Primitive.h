#pragma once
//OpenGL
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STL
#include <vector>

//Engine
#include <engine/ECSCore.h>
#include <engine/Transform.h>
#include <engine/GL/Shader.h>
#include <engine/GL/Texture.h>
#include <engine/GL/Camera.h>

extern ECS ecs;

namespace engine
{
	//A class to create and store a primitive shape
	class Primitive
	{
	private:
		Primitive(float* vertices, unsigned int* indices)
		{
			//Make the Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glEnableVertexAttribArray(0);

			//Unbind all buffers and arrays
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	public:
		~Primitive()
		{
			glDeleteVertexArrays(1, &VAO);
			glDeleteVertexArrays(1, &VBO);
			glDeleteVertexArrays(1, &EBO);
		}

		static Primitive Line(float x, float y)
		{
			float* vertices = new float[]
			{
				0, 0,
				1, 1,
			};
			unsigned int* indices = new unsigned int[]
			{
				0, 1
			};

			return Primitive(vertices, indices);
		}

		unsigned int VAO, VBO, EBO;
	};

	//Primitive Renderer Component
	//They consist of only a primitive shape and a color, no texture
	struct PrimitiveRenderer
	{
		Primitive* primitive;
		Vector3 color;
		bool enabled = true;
		bool uiElement = false;
	};


	//Primitive Render system
	//Requires PrimitiveRenderer and Transform
	class PrimitiveRenderSystem : public System
	{
	public:
		PrimitiveRenderSystem()
		{
			//The default 3D model shader
			defaultShader = new Shader(
				R"(
				#version 330 core
				layout(location = 0) in vec3 aPos;

				uniform mat4 model;
				uniform mat4 view;
				uniform mat4 projection;

				void main()
				{
					gl_Position = projection * view * model * vec4(aPos, 1.0);
				}
				)",
				R"(
				#version 330 core
				out vec4 FragColor;

				uniform vec4 color;

				void main()
				{    
					FragColor = color;
				}
				)", false);
		}

		void Update(Camera* cam)
		{
			//For each entity
			for (const Entity& entity : entities)
			{
				//Get relevant components
				Transform& transform = ecs.getComponent<Transform>(entity);
				PrimitiveRenderer& primitiveRenderer = ecs.getComponent<PrimitiveRenderer>(entity);

				if (!primitiveRenderer.enabled)
					continue;

				defaultShader->use();

				//Bind the right VAO after tilemap
				glBindVertexArray(primitiveRenderer.primitive->VAO);

				//Create the model matrix
				glm::mat4 model = glm::mat4(1.0f);
				//Position
				model = glm::translate(model, transform.position.ToGlm());
				//X, Y, Z euler rotations
				model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				//Scale
				model = glm::scale(model, transform.scale.ToGlm());

				//Give the shader the model matrix
				unsigned int modelLoc = glGetUniformLocation(defaultShader->ID, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

				//Give the shader the primitive's color
				unsigned int projLoc = glGetUniformLocation(defaultShader->ID, "color");
				//glUniform4f(modelLoc, 1, GL_FALSE, glm::vec4(primitiveRenderer.color.ToGlm(), 0));

				//Get the view and projection locations
				unsigned int viewLoc = glGetUniformLocation(defaultShader->ID, "view");
				unsigned int projLoc = glGetUniformLocation(defaultShader->ID, "projection");

				if (!primitiveRenderer.uiElement)
				{
					//Give the shader the camera's view matrix
					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam->GetViewMatrix()));

					//Give the shader the camera's projection matrix
					glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam->GetProjectionMatrix()));
				}
				else
				{
					//Give the shader a constant view matrix
					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

					//Give the shader a constant projection matrix
					glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
				}
			}
		}

		Shader* defaultShader;
	};
}