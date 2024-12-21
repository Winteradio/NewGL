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

        struct TypeCompare
        {
            bool operator()(const Type& _lhs, const Type& _rhs) const
            {
                return static_cast<UINT8>(_lhs) < static_cast<UINT8>(_rhs);
            }
        };

    public :
        Shader() = delete;
        Shader(const Shader& other);
        Shader(const Type type, const std::string& filePath);
        Shader(Shader&& other);
        virtual ~Shader();

        Shader& operator=(const Shader& other);
        Shader& operator=(Shader&& other);

    public :
        bool Init();
        const UINT32 GetShaderID() const { return m_ShaderID; }
        const Type GetShaderType() const { return m_ShaderType; }

    private :
        const UINT32 ConvertShaderType();
        const std::string GetShaderSource();

    private :
        Type m_ShaderType;
        std::string m_FilePath;
        UINT32 m_ShaderID;
};

#endif // __SHADER_H__