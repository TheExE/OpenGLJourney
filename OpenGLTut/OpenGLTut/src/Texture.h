#pragma once
#include <string>

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

private:
	unsigned int m_rendererId;
	unsigned char* m_localBuffer;
	int m_width;
	int m_height;
	int m_bitsPerPixel;	
	std::string m_filePath;
};
