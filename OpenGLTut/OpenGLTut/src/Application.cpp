#include <GL/glew.h>
#include <GLFW//glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if(!(x)) __debugbreak()

// #x in define turns x into a string
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLCheckError(#x, __FILE__, __LINE__))


static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckError(const char* function, const char* file, int line)
{
	while (const GLenum error = glGetError())
	{
		std::cout << "OpenGL error: ( " << error << " )" << " Function: " << function
			<< " File: " << file << " Line: " << line << std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSoruce
{
	std::string vertexSource;
	std::string fragmentSource;
};

static ShaderProgramSoruce ParseShader(const std::string& filePath)
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
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

static unsigned int CreateShaders(const std::string& vertexShader, const std::string& fragmentShader)
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

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Super duper cool window !", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	/* Initialize GLEW once we have a valid openGL context */
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error initializing GLEW !" << std::endl;
	}

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) <<
		std::endl;

	const unsigned int SIZE = 8;
	float positions[SIZE] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f, 0.5f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};


	// Bind vertex object array
	unsigned int vaoId;
	GLCall(glGenVertexArrays(1, &vaoId));
	GLCall(glBindVertexArray(vaoId));

	// Position buffer
	unsigned int bufferId;
	GLCall(glGenBuffers(1, &bufferId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, SIZE * sizeof(float),
		positions, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

	// Index buffer
	unsigned int indexBufferId;
	GLCall(glGenBuffers(1, &indexBufferId));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
		indices, GL_STATIC_DRAW));

	const ShaderProgramSoruce& shaderProgram =
		ParseShader("res/shaders/Basic.shader");
	const unsigned int shaderProgrammId = CreateShaders(shaderProgram.vertexSource,
		shaderProgram.fragmentSource);

	GLCall(glUseProgram(shaderProgrammId));

	GLCall(const int location = glGetUniformLocation(shaderProgrammId, "u_color"));
	ASSERT(location != -1);
	
	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(shaderProgrammId));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

		GLCall(glBindVertexArray(vaoId));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		r += increment;
		if(r > 1)
		{
			increment = -0.05f;
		}
		else if(r < 0)
		{
			increment = 0.05f;
		}
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(shaderProgrammId));
	glfwTerminate();

	return 0;
}
