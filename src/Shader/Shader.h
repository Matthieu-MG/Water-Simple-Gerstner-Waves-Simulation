#pragma once
#ifndef SHADER_H
#define SHADER_H


#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "src/Texture/Texture.h"
#include <vector>
#include <unordered_map>

class Shader
{
public:

	// the program id
	unsigned int ID;

	// DO NOT USE, ONLY DECLARED PUBLIC FOR UNORDERED_MAP REQUIREMENTS
	Shader() = default;
	// constructor reads and builds the shader
	Shader(const char* vertexShader, const char* fragmentShader);
	// use/activate the shader
	void use();
	//utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, glm::vec2 vector) const;
	void setVec3(const std::string& name, glm::vec3 vector) const;
	void setMat4(const std::string& name, glm::mat4* matrix) const;
	bool UniformExists(const std::string& name, int value) const;

private:
	void CheckCompilationErrors(unsigned int ID, std::string type);
};

class ShaderManager
{
public:
	static std::unordered_map<const char*, Shader> Shaders;
	static void AddShader(const char* v, const char* f, const char* name);
private:
	ShaderManager();
};
#endif
