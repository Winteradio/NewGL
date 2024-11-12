#ifndef __SHADER_H__
#define __SHADER_H__

#include "Type.h"

#include <string>

class Shader
{
    public :
        enum class Type : UINT8
        {
            NONE = 0,
            VERTEX,
            FRAGMENT,
            GEOMETRY,
            HULL,
            COMPUTE
        };

    public :
        Shader() = delete;
        Shader(const Type type, const std::string& filePath);
        virtual ~Shader();

    public :
        bool Init();
        UINT32 GetShaderID() { return m_ShaderID; }

    private :
        const UINT32 GetShaderType();
        const std::string GetShaderSource();

    private :
        Type m_ShaderType;
        std::string m_FilePath;
        UINT32 m_ShaderID;
};

#endif // __SHADER_H__