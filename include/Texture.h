#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Type.h"

#include <string>

class Texture
{
	public :
		Texture() = delete;
		Texture(const Texture& other);
		Texture(const std::string& filePath);
		Texture(Texture&& other);
		virtual ~Texture();

	public :
		bool Init();
		const UINT32 GetTextureID() const { return m_TextureID; }

	private :
		UINT32 m_TextureID;
		std::string m_FilePath;
};

#endif // __TEXTURE_H__