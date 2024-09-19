#include "Texture.h"

Texture2D::Texture2D(const char* path, TextureType type): m_Type(type)
{
	CreateGenericTexture();

	int width, height, nrChannels;

	// Add check if jpeg flip else don't flip///
	//stbi_set_flip_vertically_on_load(true);//
	/////////////////////////////////////////
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	m_path = path;

	unsigned int channel=GL_RED;
	if (nrChannels == 3)
		channel = GL_RGB;
	else if (nrChannels == 4)
		channel = GL_RGBA;
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, channel, width, height, 0, channel, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG("ERROR: COULD LOAD IMAGE - " << path);
	}
	stbi_image_free(data);
}

Texture2D::Texture2D(unsigned int width, unsigned int height, int internal_format, int format)
{
	CreateGenericTexture();
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
}

Texture2D::~Texture2D()
{
	// glDeleteTextures(1, &m_RendererID);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture2D::SetActiveAndBind(uint16_t TexUnit)
{
	glActiveTexture(GL_TEXTURE0+TexUnit);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture2D::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SetWrappingMode(int wrap_s, int wrap_t)
{
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
}

void Texture2D::SetFilters(int min_filter, int mag_filter)
{
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void Texture2D::CreateGenericTexture()
{
	glGenTextures(1, &m_RendererID);
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
