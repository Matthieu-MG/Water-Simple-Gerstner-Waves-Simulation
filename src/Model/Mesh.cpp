#include "Mesh.h"
#include "Model.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, unsigned int materialIndex) 
	: m_MaterialIndex(materialIndex)
{
	// Instancing Buffers and VAO on the heap to prevent their destructor from being called at the end of this method 
	m_vao = std::make_shared<VertexArray>();
	m_vao->Bind();

	// IMPLEMENT ASSERT WHETHER VERTICES HAS DATA

	m_vbo = std::make_shared<VertexBuffer>((float*)&vertices[0], vertices.size() * sizeof(Vertex));
	m_ebo = std::make_shared<IndexBuffer>(&indices[0], indices.size() * sizeof(unsigned int));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_vao->AddBuffer(*m_vbo, layout);

	m_vao->Unbind();
	m_IndicesSize = indices.size();
}

void Mesh::Draw(Model* parent)
{
	parent->m_materials[m_MaterialIndex].Draw();
	glActiveTexture(GL_TEXTURE0);

	m_vao->Bind();
	glDrawElements(GL_TRIANGLES, m_IndicesSize, GL_UNSIGNED_INT, 0);
	m_vao->Unbind();
}

