#pragma once

#include "Mesh.h"
#include "src/Shader/Shader.h"
#include "src/Texture/Texture.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>
#include <string>
#include "src/Texture/Material.h"
#include <optional>

class Model {
public:
	Model(const char* path, bool batched=false);
	void Draw();
	std::vector<const char*> TextureUniformNames;
	std::vector<Material> m_materials;

private:
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessNode(aiNode* node, const aiScene* scene);

	void LoadBatchedMeshes(const aiScene* scene);
	void LoadSceneMeshes(const aiScene* scene);
	
	Mesh CreateBatchedMesh(std::vector<aiMesh*>& meshes, unsigned int MaterialIndex);

	void LoadMeshVertAndIndices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, aiMesh* mesh, unsigned int offset=0u);

	int GetMaterial(unsigned int Index);
	std::vector<Texture2D>   LoadTextures(aiMaterial* mat ,aiTextureType MatType, TextureType TexType);
	std::optional<Texture2D> LoadTexture(aiMaterial* mat, aiTextureType MatType, TextureType TexType);

	std::vector<Mesh> m_meshes;
	size_t offset;

	// Find a way to dispose of them after loading
	std::vector<Texture2D> m_globalTextures;
	uint16_t m_nrDiffuse;
	uint16_t m_nrSpecular;
	std::string m_Directory;
};