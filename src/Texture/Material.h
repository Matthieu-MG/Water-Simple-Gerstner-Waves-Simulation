#pragma once

#include "src/Shader/Shader.h"
#include "src/Texture/Texture.h"
#include <vector>

class Material
{
public:
	Material(std::vector<Texture2D>& textures, unsigned int ID);
	void Draw();
	void BindShader(Shader* shader);
	inline unsigned int GetID() { return m_Model_matID; };
	inline Shader& GetShader() { return *m_Shader; }

private:
	unsigned int m_Model_matID;
	Shader* m_Shader;
	std::vector<Texture2D> m_Textures;
};