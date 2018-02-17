#include "IndexBuffer.h"
#include <GL/glew.h>
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* pIndexes, unsigned int count) :
	m_count(count)
{
	// On some platforms this may fail
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_rendererId));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
		pIndexes, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererId));
}

void IndexBuffer::Bind() const 
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
