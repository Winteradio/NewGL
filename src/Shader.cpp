#include "Shader.h"

#include "glad/gl.h"
#include "glad/wgl.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const Type type, const std::string& filePath)
    : m_ShaderType(type)
    , m_FilePath(filePath)
    , m_ShaderID(0)
{}

Shader::~Shader()
{}

bool Shader::Init()
{
    const UINT32 shaderType = GetShaderType();
    const std::string stShaderSource = GetShaderSource();

    if (GL_NONE == shaderType || stShaderSource.empty())
    {
        std::cout << "Failed to create shader, the shader info is invalid" << std::endl;
        return false;
    }

    const char* pShaderSource = stShaderSource.c_str();

    m_ShaderID = glCreateShader(shaderType);
    glShaderSource(m_ShaderID, 1, &pShaderSource, NULL);
    glCompileShader(m_ShaderID);

    INT32 success;
    char infoLog[512];

    glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_ShaderID, 512, NULL, infoLog);
        std::cout << "ERROR: " << infoLog << std::endl;
        return false;
    }

    return true;
}

const UINT32 Shader::GetShaderType()
{
    UINT32 shaderType = GL_NONE;
    if (Type::VERTEX == m_ShaderType)
    {
        shaderType = static_cast<UINT32>(GL_VERTEX_SHADER);
    }
    else if (Type::FRAGMENT == m_ShaderType)
    {
        shaderType = static_cast<UINT32>(GL_FRAGMENT_SHADER);
    }
    else if (Type::GEOMETRY == m_ShaderType)
    {
        shaderType = static_cast<UINT32>(GL_GEOMETRY_SHADER);
    }
    else if (Type::COMPUTE == m_ShaderType)
    {
        shaderType = static_cast<UINT32>(GL_COMPUTE_SHADER);
    }
    else
    {
        // None
    }

    return shaderType;
}

const std::string Shader::GetShaderSource()
{
    if (m_FilePath.empty())
    {
        std::cout << "Failed to open shader file, cause the file path is empty" << std::endl;
        return std::string();
    }

    std::cout << m_FilePath << std::endl;

    std::ifstream file(m_FilePath);
    if (!file.is_open())
    {
        std::cout << "Failed to open shader file" << std::endl;
        return std::string();
    }

    std::stringstream fileBuffer;
    fileBuffer << file.rdbuf();

    std::string shaderCode = fileBuffer.str();

    std::cout << "Code : " << shaderCode << std::endl;
    return shaderCode;
}