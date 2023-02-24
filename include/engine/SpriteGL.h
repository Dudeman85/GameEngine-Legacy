#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <engine/ECSCore.h>
#include <engine/Transform.h>
#include <engine/GL/Shader.h>
#include <engine/GL/Texture.h>
#include <engine/GL/Camera.h>

extern ECS ecs;

namespace engine
{
	//Sprite component
	struct Sprite
	{
		Texture* texture;
		Shader* shader = nullptr;
	};

	//Render system
	//Requires Sprite and Transform
	class RenderSystem : public System
	{
	public:
		RenderSystem()
		{
			//Set the screen clear color to black
			glClearColor(0, 0, 0, 1.0f);

			//Enable transparency
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			//Create the default shader
			defaultShader = Shader();

			//Rectangle vertices start at top left and go clockwise to bottom left
			float vertices[] = {
				//Positions		  Texture Coords
				 1.f,  1.f, 0.0f, 1.0f, 1.0f, // top right
				 1.f, -1.f, 0.0f, 1.0f, 0.0f, // bottom right
				-1.f, -1.f, 0.0f, 0.0f, 0.0f, // bottom left
				-1.f,  1.f, 0.0f, 0.0f, 1.0f, // top left 
			};
			//Indices to draw a rectangle from two triangles
			unsigned int indices[] = {
				0, 1, 2, //1st trangle
				0, 2, 3, //2nd triangle
			};

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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			//Configure Vertex attribute at location 1 aka texture coords
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		//Call this every frame
		void Update(Camera* cam)
		{
			//Clear the screen
			glClear(GL_COLOR_BUFFER_BIT);

			//Only need to bind the VAO once
			glBindVertexArray(VAO);

			for (auto const& entity : entities)
			{
				//Get relevant components
				Sprite& sprite = ecs.getComponent<Sprite>(entity);
				Transform& transform = ecs.getComponent<Transform>(entity);

				//If a shader has been specified for this sprite use it, else use the default
				Shader shader = defaultShader;
				if (sprite.shader)
					shader = *sprite.shader;
				shader.use();

				//Create the model matrix
				glm::mat4 model = glm::mat4(1.0f);
				//Position
				model = glm::translate(model, glm::vec3(transform.x, transform.y, transform.z));
				//Scale
				model = glm::scale(model, glm::vec3(transform.xScale, transform.yScale, transform.zScale));
				//X, Y, Z euler rotations
				model = glm::rotate(model, transform.xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, transform.yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, transform.zRotation, glm::vec3(0.0f, 0.0f, 1.0f));

				//Give the shader the model matrix
				unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

				//Give the shader the view matrix
				unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam->GetViewMatrix()));

				//Give the shader the projection matrix
				unsigned int projLoc = glGetUniformLocation(shader.ID, "projection");
				glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam->GetProjectionMatrix()));

				//Bing the texture
				glBindTexture(GL_TEXTURE_2D, sprite.texture->GetID());

				//Draw the sprite
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}

		//Set the screens clear color to given normalized rgb
		void SetBackgroundColor(float r, float g, float b)
		{
			glClearColor(r, g, b, 1.0f);
		}

	private:
		unsigned int VAO, VBO, EBO;
		Shader defaultShader;
	};
}