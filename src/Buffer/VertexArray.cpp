#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
	Bind();
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	// Passing in by reference prevents copy and destruction of VertexBuffer in the GPU, once the method ends
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	const unsigned int stride = layout.GetStride();

	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, stride, (void*)offset);
		glEnableVertexAttribArray(i);
		offset += elements[i].count * VertexBufferLayout::GetTypeSize(elements[i].type);
	}
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
