#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_rendererId));
}

VertexArray::~VertexArray()
{
	Unbind();
	GLCall(glDeleteVertexArrays(1, &m_rendererId));
}

void VertexArray::AddVertexBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout)
{
	Bind();
	buffer.Bind();

	const auto& elements = layout.GetElements(); // Get all the attribute layout for this buffer
	unsigned int offset = 0;
	for(unsigned int index = 0; index < elements.size(); index++)
	{
		const VertexBufferElement& element = elements[index];
		GLCall(glEnableVertexAttribArray(index));
		
		GLCall(glVertexAttribPointer(index, element.count, element.type,
			element.bIsNormalized, layout.GetStride(), reinterpret_cast<const void*>(offset)));
		
		const unsigned int size = VertexBufferElement::GetSizeOfType(element.type) *
			element.count;
		offset += size;
	}	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_rendererId));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
