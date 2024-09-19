#pragma once

#include <glad/glad.h>
#include <vector>
#include "vendor/stb_image/stb_image.h"
#include "src/GeneralUtils.h"

class TextureCubeMap
{
public:
	TextureCubeMap(std::vector<const char*>& faces);
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
};