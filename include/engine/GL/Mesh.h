#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <engine/GL/Shader.h>

namespace engine
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct ModelTexture
	{
		unsigned int id;
		std::string type;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> textures);
		void Draw(Shader& shader);

		//Mesh data
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<ModelTexture> textures;

	private:
		void setupMesh();

		//Buffer Data
		unsigned int VAO, VBO, EBO;
	};
}