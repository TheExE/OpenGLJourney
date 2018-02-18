#include "Shader.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
{
	const ShaderProgramSoruce& shaderProgram = ParseShader(filepath);
	m_rendererId = CreateShaders(shaderProgram.vertexSource,
		shaderProgram.fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_rendererId));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_rendererId));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1,
	float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	/* location is int because
	* in missing uniform scenario it will
	* contain negative value
	*/
	int location;
	if(m_uniformLocations.find(name) == m_uniformLocations.end())
	{
		GLCall(location = glGetUniformLocation(m_rendererId, name.c_str()));
		if (location == -1)
		{
			std::cout << "Warning: Uniform " << name
				<< " location does not exist !";
		}
		else
		{
			m_uniformLocations[name] = location;
		}
	}
	else
	{
		location = m_uniformLocations[name];
	}

	return location;
}

ShaderProgramSoruce Shader::ParseShader(const std::string& filePath)
{
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
	};

	std::fstream stream(filePath);
	std::string line;
	std::stringstream stringStreams[2];
	ShaderType type = ShaderType::NONE;

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			stringStreams[static_cast<int>(type)] << line << std::endl;
		}
	}

	return { stringStreams[static_cast<int>(ShaderType::VERTEX)].str(),
		stringStreams[static_cast<int>(ShaderType::FRAGMENT)].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		// Get the error message
		int lenght;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
		char* message = static_cast<char*>(alloca(lenght * sizeof(char)));
		GLCall(glGetShaderInfoLog(id, lenght, &lenght, message));

		std::cout << "Failed to compile shader: " << ((id == GL_VERTEX_SHADER) ? "vertex" : "fragment")
			<< " " << message << std::endl;

		GLCall(glDeleteShader(id));
		id = -1;
	}

	return id;
}

unsigned int Shader::CreateShaders(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(const unsigned int programId = glCreateProgram());
	const unsigned int vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShader);
	const unsigned int fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(programId, vertexShaderId));
	GLCall(glAttachShader(programId, fragmentShaderId));

	GLCall(glLinkProgram(programId));
	GLCall(glValidateProgram(programId));

	GLCall(glValidateProgram(programId));
	GLCall(glDeleteShader(fragmentShaderId));

	return programId;
}

