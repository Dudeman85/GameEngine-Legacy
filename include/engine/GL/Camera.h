#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine
{
	class Camera
	{
	public:
		Camera(float w, float h, glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f))
		{
			width = w;
			height = h;
			position = pos;
		}

		//Sets the camera position in word space
		void SetPosition(float x, float y, float z)
		{
			//I really have no idea why these have to be divided by 2, but otherwise the coordinates don't match the sprites
			position.x = x / 2;
			position.y = y / 2;
			position.z = z / 2;
		}

		//Move the camera in world space. Camera's origin is in its center
		void Translate(float dx, float dy, float dz = 0.f)
		{
			//I really have no idea why these have to be divided by 2, but otherwise the coordinates don't match the sprites
			position.x += dx / 2;
			position.y += dy / 2;
			position.z += dz / 2;
		}

		//Sets the width and height of the camera
		void SetDimensions(float w, float h)
		{
			width = w;
			height = h;
		}

		//Get the projection matrix for this camera. is perspective is set to true, use perspective projection instead of orthographic
		glm::mat4 GetProjectionMatrix()
		{
			if (perspective)
				return glm::perspective(glm::radians(fov), width / height, 0.01f, farPlane);
			else
				return glm::ortho(position.x - width / 2, position.x + width / 2, position.y - height / 2, position.y + height / 2, 0.01f, farPlane);
		}
	

		//Get the model matrix for this camera, aka the view matrix
		glm::mat4 GetViewMatrix()
		{
			return glm::translate(glm::mat4(1.0f), -position);
		}

		glm::vec3 position;
		bool perspective = false;
		float width;
		float height;
		float farPlane = 1000;
		float fov = 45;
	};
}