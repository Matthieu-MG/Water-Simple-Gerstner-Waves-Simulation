#include "Shader.h"

// EMPLACE SHADERS VIA SHADERMANAGER
std::unordered_map<const char*, Shader> ShaderManager::Shaders;

void ShaderManager::AddShader(const char* v, const char* f, const char* name)
{
	Shaders.emplace(name, Shader(v, f));
}

// constructor generates the shader on the fly
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{

	// 1. Retrieve the vertex/fragment source from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw objections
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer content into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. Compile shaders
	unsigned int vertex, fragment;
		
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompilationErrors(vertex, "VERTEX");

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompilationErrors(fragment, "FRAGMENT");

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	CheckCompilationErrors(ID, "PROGRAM");

	// Delete shaders since already linked to program
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// Sets uniform for material texture maps
	use();
	GLint AmountOfUniforms;
	GLsizei BufferSize;
	glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &AmountOfUniforms);
	glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &BufferSize);
	GLsizei length;
	GLint size;
	GLenum type;
	GLchar* name = new GLchar[BufferSize];
	for (GLint i = 0; i < AmountOfUniforms; i++)
	{
		glGetActiveUniform(ID, i, BufferSize, &length, &size, &type, name);
		if (name != nullptr)
		{
			if (type == GL_SAMPLER_2D && name[0] == 'm' && name[1] == '_')
			{
				switch (name[2])
				{
					case 'd':
						std::cout << "Uniform Sampler: DIFFUSE " << name << std::endl;
						setInt(name, DIFFUSE);
						break;
					case 's':
						std::cout << "Uniform Sampler: SPECULAR" << name << std::endl;
						setInt(name, SPECULAR);
						break;
					case 'e':
						std::cout << "Uniform Sampler: EMISSIVE" << name << std::endl;
						setInt(name, EMISSIVE);
						break;
					default:
						break;
				}
			}
		}
	}

	delete[] name;
}

// activates the shader
void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, glm::vec2 vector) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y);
}

void Shader::setVec3(const std::string& name, glm::vec3 vector) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::setMat4(const std::string& name, glm::mat4* matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(*matrix));
}

bool Shader::UniformExists(const std::string& name, int value) const
{
	if (glGetUniformLocation(ID, name.c_str()) == -1)
		return false;
	else
		return true;
}

// utility function for checking compilation/linking errors.
void Shader::CheckCompilationErrors(unsigned int ID, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_ERROR: " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINKING_ERROR\n" << infoLog << std::endl;
		}
	}
}
