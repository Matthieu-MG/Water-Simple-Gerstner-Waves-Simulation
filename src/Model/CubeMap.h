#pragma once

#include "src/Texture/TextureCubeMap.h"
#include "src/Buffer/VertexArray.h"
#include "src/Shader/Shader.h"

class CubeMap
{
public:
	CubeMap(std::vector<const char*>& faces): m_Shader(ShaderManager::Shaders["CUBE_MAP"]), m_Texture(TextureCubeMap(faces))
	{
		m_vao = std::make_shared<VertexArray>();
		m_vao->Bind();
		float quad[108] =
		{
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
		};

		m_vbo = std::make_shared<VertexBuffer>(quad, 108 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);

		m_vao->AddBuffer(*m_vbo, layout);
		m_vao->Unbind();
	}

	void Draw() const
	{
        glActiveTexture(GL_TEXTURE0);
        m_Texture.Bind();
		m_Shader.use();
		m_vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
	}

    void BindCubeMapTex()
    {
        m_Texture.Bind();
    }

private:
	Shader& m_Shader;
	std::shared_ptr<VertexArray>  m_vao;
	std::shared_ptr<VertexBuffer> m_vbo;
    TextureCubeMap m_Texture;
};