#pragma once
#include <string>
#include <mat4x4.hpp>
#include "ShaderLoader.h"

class Shader
{
public:
	Shader();
	Shader(const char* vertexSrc, const char* fragmentSrc);
	~Shader() = default;

	void Bind() const;
	static void Unbind();

	void CleanUp() const;

	void SetUniform1f(const std::string& _name, float _v1) const;
	void SetUniform3f(const std::string& _name, float _v1, float _v2, float _v3) const;

	void SetUniform1i(const std::string& _name, int _v1) const;

	void SetUniformMat4f(const std::string& _name, glm::mat4& _matrix) const;

	int GetUniformLocation(const std::string& _name) const;

private:
	unsigned int m_programID{};
};

