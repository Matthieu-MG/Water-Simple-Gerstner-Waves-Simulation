#pragma once

#include <glad/glad.h>
#include <vector>
#include <stdexcept>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned int normalized;
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:

	VertexBufferLayout() : m_Stride(0) { ; }

	unsigned int GetStride() const { return m_Stride; }
	const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }

	static unsigned int GetTypeSize(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			default:
				return -1;
		}
	}

	template<typename T>
	void Push(unsigned int count)
	{ 
		std::runtime_error(false);
	}

	template <>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_Stride += count * sizeof(float);
	}

	template <>
	void Push<int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * sizeof(unsigned int);
	}
};