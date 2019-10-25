#include "Shader.h"
#include "Debug.h"

#include <vector>
#include <freeglut.h>
#include <mat4x4.hpp>
#include <detail/_vectorize.hpp>

Shader::Shader()
{
	m_programID = ShaderLoader::CreateProgram("Shaders/DefaultVertexShader.shader", "Shaders/DefaultFragmentShader.shader");
}

Shader::Shader(const char* _vertexSrc, const char* _fragmentSrc)
{
	m_programID = ShaderLoader::CreateProgram(_vertexSrc, _fragmentSrc);
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_programID));
}

void Shader::Unbind()
{
	GLCall(glUseProgram(0));
}

void Shader::CleanUp() const
{
	Unbind();
	glDeleteProgram(m_programID);
	m_shaderMap->erase(m_programID);
}

void Shader::SetUniform1f(const std::string& _name, float _v1) const
{
	GLCall(glUniform1f(GetUniformLocation(_name), _v1));
}

void Shader::SetUniform3f(const std::string& _name, float _v1, float _v2, float _v3) const
{
	GLCall(glUniform3f(GetUniformLocation(_name), _v1, _v2, _v3));
}

void Shader::SetUniform1i(const std::string& _name, int _v1) const
{
	GLCall(glUniform1i(GetUniformLocation(_name), _v1));
}

void Shader::SetUniformMat4f(const std::string& _name, glm::mat4& _matrix) const
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(_name), 1, GL_FALSE, &_matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& _name) const
{
	const int uniformLocation = GLCall(glGetUniformLocation(m_programID, _name.c_str()));

	if(uniformLocation == -1)
	{
		std::cout << "Warning! Couldn't find uniform: " << _name << std::endl;
	}

	return uniformLocation;
}
