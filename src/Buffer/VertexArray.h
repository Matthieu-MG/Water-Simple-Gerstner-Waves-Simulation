#pragma once

#include <glad/glad.h>
#include "VertexBufferLayout.h"
#include "Buffer.h"

class VertexArray
{
public:
	VertexArray();
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	~VertexArray();
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};