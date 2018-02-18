#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSoruce
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1,
		float v2, float v3);

private:

	unsigned int GetUniformLocation(const std::string& name);
	ShaderProgramSoruce ParseShader(const std::string& filePath);
	unsigned CompileShader(unsigned type, const std::string& source);
	unsigned CreateShaders(const std::string& vertexShader, const std::string& fragmentShader);

	std::unordered_map<std::string, unsigned int> m_uniformLocations;
	unsigned int m_rendererId;
};
