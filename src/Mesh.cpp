#include <engine/GL/Mesh.h>

namespace engine
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> textures)
	{
		//Set the mesh data
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		//OpenGL buffers
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		//Vertices to VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		//Draw indices to EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//Vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//Vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		//Vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		//Unbind VAO
		glBindVertexArray(0);
	}

	void Mesh::Draw(Shader& shader)
	{
		//Texture uniforms are named: uniform sampler2D texture_diffuseN, or texture_specularN
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			//Activate proper texture unit before binding
			glActiveTexture(GL_TEXTURE0 + i); 

			//Retrieve texture number and type (the N in texture_{type}N)
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			
			//Set the uniform for the material texture
			//glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str(), i);

			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		//Unbind previous texture
		glActiveTexture(GL_TEXTURE0);

		//Draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}