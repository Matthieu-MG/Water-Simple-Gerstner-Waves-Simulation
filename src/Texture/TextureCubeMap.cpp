#include "TextureCubeMap.h"

TextureCubeMap::TextureCubeMap(std::vector<const char*>& faces)
{
	glGenTextures(1, &m_RendererID);
	Bind();

	int width;
	int height;
	int nr_Channels;
	for (size_t i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i], &width, &height, &nr_Channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			LOG("ERROR: FAILED TO LOAD CUBEMAP TEXTURE FROM FILE");
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureCubeMap::Bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
}

void TextureCubeMap::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}