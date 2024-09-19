#include "Material.h"

Material::Material(std::vector<Texture2D>& textures, unsigned int ID): m_Model_matID(ID)
	, m_Shader(&ShaderManager::Shaders["BASIC_MAT"])
{
	// Should improve this copy operation
	m_Textures = textures;
}

void Material::Draw()
{
	m_Shader->use();
	for (size_t i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + m_Textures[i].GetType());
		m_Textures[i].Bind();
	}
}

void Material::BindShader(Shader* shader)
{
	m_Shader = shader;
}
