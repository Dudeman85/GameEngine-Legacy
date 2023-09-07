#pragma once
#include <engine/GL/Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

namespace engine
{
	class Model
	{
	public:
		Model(const char* path)
		{
			LoadModel(path);
		}

		//Model data
		std::vector<Mesh> meshes;

	private:
		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		//Model file path
		std::string directory;
		//Store all the already loaded textures for efficiency
		std::vector<Texture*> textures_loaded;
	};
}