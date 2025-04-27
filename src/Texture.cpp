#include "Texture.h"

#include <LogProject/Log.h>

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include <iostream>
#include <fstream>
#include <sstream>

Texture::Texture(const Texture& other)
	: m_TextureID(other.m_TextureID)
{}

Texture::Texture(const std::string& filePath)
	: m_TextureID(0)
	, m_FilePath(filePath)
{}

Texture::Texture(Texture&& other)
	: m_TextureID(other.m_TextureID)
	, m_FilePath(other.m_FilePath)
{}

Texture::~Texture()
{
	if (m_TextureID != GL_NONE)
	{
		LOGINFO() << "[Texture] Delete Texture(" << m_TextureID <<")";

		glDeleteTextures(1, &m_TextureID);

		m_TextureID = GL_NONE;
	}
}

bool Texture::Init()
{
	if (m_TextureID != GL_NONE)
	{
		LOGERROR() << "[Texture] Texture(" << m_TextureID << ") already initialized.";

		return false;
	}

	if (m_FilePath.empty())
	{
		LOGERROR() << "[Texture] Failed to open texture file, cause the file path is empty";

		return false;
	}

	LOGINFO() << "[Texture] " << m_FilePath << "";

	std::ifstream file(m_FilePath);
	if (!file.is_open())
	{
		LOGERROR() << "[Texture] Failed to open texture file";

		return false;
	}

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(m_FilePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOGERROR() << "[Texture] Failed to load texture: " << m_FilePath;

		return false;
	}

	return true;
}