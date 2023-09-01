#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <engine/GL/Shader.h>
#include <glm/gtc/type_ptr.hpp>
#include <engine/GL/Texture.h>

namespace engine
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures);
		void Draw(Shader* shader);

		//Mesh data
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;

	private:
		//Buffer Data
		unsigned int VAO, VBO, EBO;
	};
}