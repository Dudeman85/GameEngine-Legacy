#pragma once
//OpenGL
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STL
#include <map>
#include <vector>

//Engine
#include <engine/ECSCore.h>
#include <engine/Transform.h>
#include <engine/GL/Shader.h>
#include <engine/GL/Texture.h>
#include <engine/GL/Camera.h>

extern ECS ecs;

using namespace std;

namespace engine
{
	//Sprite component
	struct Sprite
	{
		Texture* texture;
		Shader* shader = nullptr;
	};

	//Animation struct. Not a component
	struct Animation
	{
		Animation() {};
		Animation(vector<Texture*> animationTextures, vector<int> animationDelays)
		{
			assert(animationTextures.size() == animationDelays.size() && "Failed to create animation! Number of frames and delays do not match!");
			textures = animationTextures;
			delays = animationDelays;
			length = animationDelays.size();
		};
		vector<Texture*> textures;
		vector<int> delays;
		unsigned int length = 0;
	};

	//Animator component
	struct Animator
	{
		map<string, Animation> animations;

		string currentAnimation;
		int animationFrame = 0;
		bool repeatAnimation = false;
		bool playingAnimation = false;

		float animationTimer = 0;
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

		//Renders evrything. Call this every frame
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

				//Bind the texture
				if(sprite.texture)
					sprite.texture->Use();

				//Draw the sprite
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				//Unbind the texture
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		//Set the screens clear color to given normalized rgb
		static void SetBackgroundColor(float r, float g, float b)
		{
			glClearColor(r, g, b, 1.0f);
		}

	private:
		unsigned int VAO, VBO, EBO;
		Shader defaultShader;
	};
	
	//Animator system
	//Requires Animator and Sprite
	class AnimationSystem : public System
	{
	public:
		//Update every entity with relevant components
		void Update(float deltaTime)
		{
			//Delta time in milliseconds
			deltaTime *= 1000; 

			//For each entity that has the required components
			for (auto const& entity : entities)
			{
				//Get the relevant components from entity
				Animator& animator = ecs.getComponent<Animator>(entity);

				//If the entity is currently playing an animation
				if (animator.playingAnimation)
				{
					animator.animationTimer += deltaTime;

					//If enough time (defined by animation) has passed advance the animation frame
					if (animator.animationTimer >= animator.animations[animator.currentAnimation].delays[animator.animationFrame])
					{
						AdvanceFrame(entity);
					}
				}
			}
		}

		//Advance to the next animation frame of current animation
		static void AdvanceFrame(Entity entity)
		{
			//Get the relevant components from entity
			Animator& animator = ecs.getComponent<Animator>(entity);
			Sprite& sprite = ecs.getComponent<Sprite>(entity);

			//Change Sprites texture
			sprite.texture = animator.animations[animator.currentAnimation].textures[animator.animationFrame];

			animator.animationFrame++;
			animator.animationTimer = 0;

			//If end of animation has been reached go to start or end animation
			if (animator.animationFrame >= animator.animations[animator.currentAnimation].length)
			{
				animator.animationFrame = 0;
				animator.animationTimer = 0;

				//End the animation if it is not set to repeat
				if (!animator.repeatAnimation)
				{
					animator.playingAnimation = false;
					animator.currentAnimation = "";
					return;
				}
			}
		}

		//Add animations to entity, they will be accessible by given names
		static void AddAnimations(Entity entity, vector<Animation> animations, vector<string> names)
		{
			if (animations.size() > names.size())
				throw("Not enough names given for each animation!");

			Animator& animator = ecs.getComponent<Animator>(entity);

			//For each animation to add
			for (size_t i = 0; i < animations.size(); i++)
			{
				animator.animations.insert({ names[i], animations[i] });
			}
		}

		//Add an animation to entity, it will be accessibl by given name
		static void AddAnimation(Entity entity, Animation animation, string name)
		{
			Animator& animator = ecs.getComponent<Animator>(entity);

			//Add the animation indexed by given name
			animator.animations.insert({ name, animation });
		}

		//Play an animation, optionally set it to repeat
		static void PlayAnimation(Entity entity, string animation, bool repeat = false)
		{
			Animator& animator = ecs.getComponent<Animator>(entity);

			if (animator.animations.find(animation) == animator.animations.end())
			{
				cout << "No animation named \"" << animation << "\" was found in this entity.";
				return;
			}

			animator.currentAnimation = animation;
			animator.animationFrame = 0;
			animator.repeatAnimation = repeat;
			animator.playingAnimation = true;
			animator.animationTimer = 0;

			//Immediately advance to 1st frame of animation
			AdvanceFrame(entity);
		}

		//Stop an animation, optionally provide the specific animation to stop
		static void StopAnimation(Entity entity, string animation = "")
		{
			Animator& animator = ecs.getComponent<Animator>(entity);

			//If trying to stop animation that is not playing, return without doing anything
			if (animation != "")
				if (animator.currentAnimation != animation)
					return;

			animator.currentAnimation = "";
			animator.animationFrame = 0;
			animator.animationTimer = 0;
			animator.playingAnimation = false;
		}
	};
}