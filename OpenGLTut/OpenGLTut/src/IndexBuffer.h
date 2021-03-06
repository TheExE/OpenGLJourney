#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* pIndexes, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const ;

	inline unsigned int getCount() const { return m_count; }

private:
	unsigned int m_rendererId;
	unsigned int m_count;
};