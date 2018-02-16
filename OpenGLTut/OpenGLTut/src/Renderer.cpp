#include "Renderer.h"

#include <GL/glew.h>
#include <iostream>


void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function, const char* file, int line)
{
	while (const GLenum error = glGetError())
	{
		std::cout << "OpenGL error: ( " << error << " )" << " Function: " << function
			<< " File: " << file << " Line: " << line << std::endl;
		return false;
	}
	return true;
}