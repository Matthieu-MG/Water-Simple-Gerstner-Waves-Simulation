#pragma once

#include "src/Shader/Shader.h"
#include "src/Buffer/VertexArray.h"
#include "src/Buffer/Buffer.h"
#include "src/Buffer/VertexBufferLayout.h"
#include "glm/glm.hpp"
#include <vector>
#include "src/Texture/Texture.h"
#include <memory>

// To prevent circular dependency with Model.h
class Model;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	// Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture2D>& Textures);
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, unsigned int materialIndex);
	void Draw(Model* parent);
private:
	unsigned int m_MaterialIndex;
	uint32_t m_IndicesSize;
	std::shared_ptr<VertexArray> m_vao;
	std::shared_ptr<VertexBuffer> m_vbo;
	std::shared_ptr<IndexBuffer> m_ebo;
};