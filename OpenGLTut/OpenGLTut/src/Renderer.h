#pragma once

#define ASSERT(x) if(!(x)) __debugbreak()

// #x in define turns x into a string
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLCheckError(#x, __FILE__, __LINE__))
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);

class Renderer
{
public:
	void Draw(const VertexArray& vertexArray, 
		const IndexBuffer& indexBuffer);
	void Clear() const;
};