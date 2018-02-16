#pragma once

#define ASSERT(x) if(!(x)) __debugbreak()

// #x in define turns x into a string
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLCheckError(#x, __FILE__, __LINE__))

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);
