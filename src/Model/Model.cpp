#include "Model.h"

Model::Model(const char* path, bool batched) : m_nrDiffuse(1), m_nrSpecular(1), offset(0)
{
	// Loads model
	Assimp::Importer importer;
	// ADD JOIN_IDENTICAL_VERTICES FLAG
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | 
											  aiProcess_JoinIdenticalVertices);

	// Check for loading errors
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
	std::string tmp_path = path;
	std::string fileName = scene->GetShortFilename(path);
	fileName = fileName.substr(fileName.length()-3, fileName.find_last_of("."));
	
	if (fileName[0] == 'o' && fileName[1] == 'b' && fileName[2] == 'j')
	{
		LOG("OBJ FILE FOUND");
		offset = 1;
	}

	m_Directory = tmp_path.substr(0, tmp_path.find_last_of('\\')) + '\\';

	// Get root, process each node to add meshes
	if (!batched)
		LoadSceneMeshes(scene);
	// ERROR TODO: Model has no material, causes crash since no shader to it,
	// Should check if model has no material and assign a base one to it
	else
	{
		LoadBatchedMeshes(scene);
	}
	// ProcessNode(scene->mRootNode, scene);
	m_globalTextures.clear();
	importer.FreeScene();

	LOG("NUMBER OF MESHES:" << m_meshes.size());
	LOG("NUMBER OF MATERIALS:" << m_materials.size());
}

void Model::Draw()
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Draw(this);
		// 144 bytes each (mesh) || 96 bytes for each Texture || 32 bytes for Texture Vector (mesh)
		// 144 bytes each (mesh) || 56 bytes for each Texture || 32 bytes for Texture Vector (mesh)
		// 88 bytes each (mesh) ||
		// 56 bytes each (mesh) ||
		// LOG( "Size of (mesh): " << sizeof(m_meshes[i]));
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	vertices.reserve(mesh->mNumVertices);

	std::vector<unsigned int> indices;
	indices.reserve(static_cast<size_t>(mesh->mNumFaces * 3u)); 
	std::vector<Texture2D> textures;

	unsigned int MaterialIndex;

	LoadMeshVertAndIndices(vertices, indices, mesh);

	// SET MATERIAL // TEXTURES
	if (mesh->mMaterialIndex >= 0)
	{
		// Loop through materials vector, if no find (-1), create new material
		int Mat_Index = GetMaterial(mesh->mMaterialIndex);
		if (Mat_Index == -1)
		{
			// Create Material
			textures.reserve(2);
			auto diffuseOpt = LoadTexture(scene->mMaterials[mesh->mMaterialIndex],  aiTextureType_DIFFUSE,  DIFFUSE);
			auto specularOpt = LoadTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, SPECULAR);
			auto emissiveOpt = LoadTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_EMISSIVE, EMISSIVE);
			
			if (diffuseOpt.has_value())
				textures.push_back(diffuseOpt.value());
			if (specularOpt.has_value())
				textures.push_back(specularOpt.value());
			if (emissiveOpt.has_value())
				textures.push_back(emissiveOpt.value());
			// Get Textures
			Material mat(textures, mesh->mMaterialIndex);
			// Push material to vector
			m_materials.push_back(mat);
			MaterialIndex = m_materials.size() - 1;
		}
		else
		{
			// Set Material Index
			MaterialIndex = Mat_Index;
		}
	}

	return Mesh(vertices, indices, MaterialIndex);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Adds all meshes of current node to Model Mesh Vector
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}
	// Same process goes for child nodes
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

// Batches meshes of same material together, to reduce draw calls when rendering
void Model::LoadBatchedMeshes(const aiScene* scene)
{
	// Start at 1 for obj, 0 for glTF
	int iteration_offset = offset - 1;

	for (size_t i = offset; i < static_cast<size_t>(scene->mNumMaterials + iteration_offset); i++)
	{
		// Create new material
		std::vector<Texture2D> textures;
		textures.reserve(2);
		auto diffuseOpt  = LoadTexture(scene->mMaterials[i], aiTextureType_DIFFUSE, DIFFUSE);
		auto specularOpt = LoadTexture(scene->mMaterials[i], aiTextureType_SPECULAR, SPECULAR);
		if (diffuseOpt.has_value())
			textures.push_back(diffuseOpt.value());
		if (specularOpt.has_value())
			textures.push_back(specularOpt.value());
		Material mat(textures, i);
		m_materials.push_back(mat);

		// Loop through number of meshes
		std::vector<aiMesh*> BatchedMeshes;
		for (size_t j = 0; j < scene->mNumMeshes; j++)
		{
			if (scene->mMeshes[j]->mMaterialIndex == i)
				BatchedMeshes.push_back(scene->mMeshes[j]);
		}
		Mesh batchedMesh = CreateBatchedMesh(BatchedMeshes, i - offset);
		m_meshes.push_back(batchedMesh);
	}
}

void Model::LoadSceneMeshes(const aiScene* scene)
{
	m_meshes.reserve(scene->mNumMeshes);
	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		m_meshes.push_back(ProcessMesh(scene->mMeshes[i], scene));
	}
}

Mesh Model::CreateBatchedMesh(std::vector<aiMesh*>& meshes, unsigned int MaterialIndex)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VertexOffset = 0u;

	// Loop through meshes
	for (size_t i = 0; i < meshes.size(); i++)
	{
		LoadMeshVertAndIndices(vertices, indices, meshes[i], offset);
		// Add offset
		VertexOffset += meshes[i]->mNumVertices;
	}

	return Mesh(vertices, indices, MaterialIndex);
}

void Model::LoadMeshVertAndIndices(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, aiMesh* mesh, unsigned int offset)
{
	// Load Vertex
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}
		else
		{
			vertex.Normal = glm::vec3(0.0f);
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	// Load Indices
	for (size_t j = 0; j < mesh->mNumFaces; j++)
	{
		aiFace face = mesh->mFaces[j];
		assert(face.mNumIndices == 3);
		for (size_t k = 0; k < face.mNumIndices; k++)
		{
			indices.push_back(face.mIndices[k] + offset);
		}
	}
}

int Model::GetMaterial(unsigned int Index)
{
	for (size_t i = 0; i < m_materials.size(); i++)
	{
		if (Index == m_materials[i].GetID())
		{
			return i;
		}
	}
	return -1;
}

std::vector<Texture2D> Model::LoadTextures(aiMaterial* mat, aiTextureType MatType, TextureType TexType)
{
	std::vector<Texture2D> textures;
	textures.reserve(mat->GetTextureCount(MatType));

	for (unsigned int i = 0; i < mat->GetTextureCount(MatType); i++)
	{
		bool skip = false;
		aiString str;
		mat->GetTexture(MatType, i, &str);
		std::string st = str.C_Str();
		st = m_Directory + st;

		for (size_t j = 0; j < m_globalTextures.size(); j++)
		{
			if (std::strcmp(m_globalTextures[j].GetPath(), st.c_str()) == 0)
			{
				textures.push_back(m_globalTextures[i]);
				skip = true;
				break;
			}
		}
		if(!skip)
		{
			Texture2D tex(st.c_str(), TexType);
			// Create Uniform Name
			std::string name = "";
			if (TexType == DIFFUSE) { name = "m_diffuse" + std::to_string(m_nrDiffuse); m_nrDiffuse++; }
			else if (TexType == SPECULAR) { name = "m_specular" + std::to_string(m_nrSpecular); m_nrSpecular++; }
			// Push Uniform Name to vector UniformNames
			TextureUniformNames.push_back(name.c_str());
			// Assign index to tex.Uniform_ID
			tex.SetUniformID(TextureUniformNames.size() - 1);
			textures.push_back(tex);
			m_globalTextures.push_back(tex);
		}
	}

	return textures;
}

std::optional<Texture2D> Model::LoadTexture(aiMaterial* mat, aiTextureType MatType, TextureType TexType)
{
	unsigned int i = mat->GetTextureCount(MatType);
	if(i > 0)
	{
		aiString str;
		mat->GetTexture(MatType, 0, &str);
		std::string st = str.C_Str();
		st = m_Directory + st;
		LOG(st);
		Texture2D tex(st.c_str(), TexType);
		return tex;
	}
	return std::nullopt;
}
