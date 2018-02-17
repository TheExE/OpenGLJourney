#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	bool bIsNormalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		unsigned int size = 0;
		switch (type)
		{
		case GL_FLOAT:
			size = sizeof(GLfloat);
			break;

		case GL_UNSIGNED_INT:
			size = sizeof(GLuint);
			break;

		case GL_UNSIGNED_BYTE:
			size = sizeof(GLubyte);
			break;

		default:
			ASSERT(false);
			break;
		}

		return size;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout();

	template <typename  T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, false });
		m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, false });
		m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, false });
		m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline unsigned int GetStride() const { return m_stride; }
	inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }

private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
};