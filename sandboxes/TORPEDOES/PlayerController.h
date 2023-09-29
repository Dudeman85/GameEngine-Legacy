#pragma once 
#include <engine/Application.h>


extern ECS ecs;

using namespace engine;
// componentes del jugador / player componenti 
struct Player {

	float maxSpeed = 1;
	float acerationSpeed = 1;


};
//payer systimemi , tranfor, BoxCollider , Rigidbody , Player , ModelRenderer
class PlayerController : public System{
public:
	void Update(GLFWwindow* window) {
		for (auto const& entity : entities) {
			Transform& transform = ecs.getComponent<Transform>(entity);

			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				transform.rotation.y += 1;
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				transform.rotation.y += -1;
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
				transform.rotation.x += -1;
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
				transform.rotation.x += 1;
		}
	}
};
