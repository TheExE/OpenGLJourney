#include "VertexBuffer.h"
#include <GL/glew.h>
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* pData, unsigned size)
{
	GLCall(glGenBuffers(1, &m_rendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size,
		pData, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererId));
}

void VertexBuffer::bind() const 
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
}

void VertexBuffer::unBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
