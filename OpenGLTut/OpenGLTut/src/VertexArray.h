#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddVertexBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_rendererId;
};
