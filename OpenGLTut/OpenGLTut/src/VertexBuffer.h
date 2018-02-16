#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* pData, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unBind() const;

private:
	unsigned int m_rendererId;
};