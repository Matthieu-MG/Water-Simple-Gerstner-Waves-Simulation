#pragma once

#include <glad/glad.h>
#include "vendor/stb_image/stb_image.h"
#include <string>
#include "src/GeneralUtils.h"

enum TextureType
{
	DIFFUSE, SPECULAR, EMISSIVE, EMISSION_COLOR
};

class Texture2D
{
public:
	Texture2D(const char* path, TextureType type);
	Texture2D(unsigned int width, unsigned int height, int internal_format, int format);
	~Texture2D();
	// Bind Methods
	void Bind() const;
	void Unbind()const;
	void SetActiveAndBind(uint16_t TexUnit);
	// Wrapping and Filters Setters
	void SetWrappingMode(int wrap_s, int wrap_t);
	void SetFilters(int min_filter, int mag_filter);
	// Getters
	inline unsigned int GetRendererID() const { return m_RendererID; }
	inline TextureType GetType() const { return m_Type; };
	// TO CHANGE SINCE WE'RE ACCESSING GARBAGE VALUES IF m_path IS INDEED NULL, MAY CAUSE ERRORS
	const char* GetPath() { if (m_path.c_str() == nullptr) { return "null"; } return m_path.c_str(); }
	// Deprecated ??
	void SetUniformID(unsigned int id) { UniformID = id; }
	unsigned int UniformID;
	std::string TextureUniform;

private:
	void CreateGenericTexture();
	TextureType m_Type;
	unsigned int m_RendererID;
	std::string m_path;
};