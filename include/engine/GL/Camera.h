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

		//Move the camera in world space. Camera's origin is in its center
		void Translate(float x, float y, float z = 0.f)
		{
			position.x += x;
			position.y += y;
			position.z += z;
		}

		//Get the projection matrix for this camera. is perspective is set to true, use perspective projection instead of orthographic
		glm::mat4 GetProjectionMatrix(bool perspective = false)
		{
			if (perspective)
				return glm::perspective(glm::radians(fov), width / height, 0.f, farPlane);
			else
				return glm::ortho(position.x - width / 2, position.x + width / 2, position.y - height / 2, position.y + height / 2, 0.f, farPlane);
		}

		//Get the model matrix for this camera, aka the view matrix
		glm::mat4 GetViewMatrix()
		{
			return glm::translate(glm::mat4(1.0f), -position);
		}

		glm::vec3 position;
		float width;
		float height;
		float farPlane = 1000;
		float fov = 45;
	};
}