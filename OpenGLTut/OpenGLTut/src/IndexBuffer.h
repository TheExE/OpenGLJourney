#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* pIndexes, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unBind() const ;

	inline unsigned int getCount() const { return m_count; }

private:
	unsigned int m_rendererId;
	unsigned int m_count;
};