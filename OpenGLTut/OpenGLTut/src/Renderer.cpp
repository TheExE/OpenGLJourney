#include <GL/glew.h>
#include <iostream>

#include "Renderer.h"

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

void Renderer::Draw(const VertexArray& vertexArray,
	const IndexBuffer& indexBuffer)
{	
	vertexArray.Bind();
	indexBuffer.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.getCount(),
		GL_UNSIGNED_INT, nullptr));	
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
