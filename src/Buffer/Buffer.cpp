#include "Buffer.h"

//////////////////////////////////////////////////////////
/////////// VERTEX BUFFER METHODS ////////////////////////
//////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(float* data, unsigned int size)
{
	glGenBuffers(1, &m_RendererID);
	Bind();

	glBufferData(GL_ARRAY_BUFFER, size, (const void*)data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////
/////////// INDEX BUFFER METHODS /////////////////////////
//////////////////////////////////////////////////////////
IndexBuffer::IndexBuffer(unsigned int* data, unsigned int size)
{
	glGenBuffers(1, &m_RendererID);
	Bind();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (const void*)data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
