#pragma once

#ifndef BUFFER_H
#define BUFFER_H

#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer(float* data, unsigned int size);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};

class IndexBuffer
{
public:
	IndexBuffer(unsigned int* data, unsigned int size);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};

#endif // !BUFFER_H
