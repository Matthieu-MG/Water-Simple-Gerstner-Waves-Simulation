#pragma once

#include "src/Shader/Shader.h"
#include "src/Buffer/Buffer.h"
#include "src/Buffer/VertexArray.h"
#include "src/Buffer/VertexBufferLayout.h"
#include <memory>

class Quad
{
public:

	Quad() : m_Shader(ShaderManager::Shaders["QUAD"])
	{
		m_vao = std::make_shared<VertexArray>();
		m_vao->Bind();
		float quad[20] =
		{
			// Position      // UV
			-0.5, -0.5, 0.0, 0.0, 0.0, // bottom left
			 0.5, -0.5, 0.0, 1.0, 0.0, // bottom right
			-0.5,  0.5, 0.0, 0.0, 1.0, // top left
			 0.5,  0.5, 0.0, 1.0, 1.0  // top right
		};

		unsigned int indices[6] =
		{
			0, 1, 3,
			0, 2, 3
		};
		m_vbo = std::make_shared<VertexBuffer>(quad, 20*sizeof(float));
		m_ebo = std::make_shared<IndexBuffer>(indices, 6 * sizeof(unsigned int));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_vao->AddBuffer(*m_vbo, layout);
		m_vao->Unbind();
	}

	void Draw() const
	{
		m_Shader.use();
		m_vao->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	Shader& m_Shader;
	std::shared_ptr<VertexArray>  m_vao;
	std::shared_ptr<VertexBuffer> m_vbo;
	std::shared_ptr<IndexBuffer>  m_ebo;
};

class ScreenQuad
{
public:

	ScreenQuad() : m_Shader(ShaderManager::Shaders["SCREEN_TEXTURE"])
	{
		m_vao = std::make_shared<VertexArray>();
		m_vao->Bind();
		float quad[20] =
		{
			// Position			// UV
			-1.0, -1.0,	0.0,	0.0, 0.0, // bottom left
			 1.0, -1.0,	0.0,	1.0, 0.0, // bottom right
			-1.0,  1.0,	0.0,	0.0, 1.0, // top left
			 1.0,  1.0,	0.0,	1.0, 1.0  // top right
		};

		unsigned int indices[6] =
		{
			0, 1, 3,
			0, 3, 2
		};
		m_vbo = std::make_shared<VertexBuffer>(quad, 20 * sizeof(float));
		m_ebo = std::make_shared<IndexBuffer>(indices, 6 * sizeof(unsigned int));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_vao->AddBuffer(*m_vbo, layout);
		m_vao->Unbind();
	}

	void Draw() const
	{
		// m_Shader.use();
		m_vao->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	Shader& m_Shader;
	std::shared_ptr<VertexArray>  m_vao;
	std::shared_ptr<VertexBuffer> m_vbo;
	std::shared_ptr<IndexBuffer>  m_ebo;
};