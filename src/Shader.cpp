#include "Shader.h"

#include <LogProject/Log.h>

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const Shader& other)
    : m_ShaderType(other.m_ShaderType)
    , m_FilePath(other.m_FilePath)
    , m_ShaderID(other.m_ShaderID)
{}

Shader::Shader(const Type type, const std::string& filePath)
    : m_ShaderType(type)
    , m_FilePath(filePath)
    , m_ShaderID(0)
{}

Shader::Shader(Shader&& other)
    : m_ShaderType(other.m_ShaderType)
    , m_FilePath(other.m_FilePath)
    , m_ShaderID(other.m_ShaderID)
{}

Shader& Shader::operator=(const Shader& other)
{
    if (this != &other)
    {
        m_ShaderType = other.m_ShaderType;
        m_FilePath = other.m_FilePath;
        m_ShaderID = other.m_ShaderID;
    }

    return *this;
}

Shader& Shader::operator=(Shader&& other)
{
    m_ShaderType = other.m_ShaderType;
    m_FilePath = other.m_FilePath;
    m_ShaderID = other.m_ShaderID;

    return *this;
}

Shader::~Shader()
{
    if (m_ShaderID != GL_NONE)
    {
        LOGINFO() << "[Shader] Delete Shader(" << m_ShaderID <<")";

        glDeleteShader(m_ShaderID);

        m_ShaderID = GL_NONE;
    }
}

bool Shader::Init()
{
    const UINT32 shaderType = ConvertShaderType();
    const std::string stShaderSource = GetShaderSource();

    if (GL_NONE == shaderType || stShaderSource.empty())
    {
        LOGERROR() << "[Shader] Failed to create shader, the shader info is invalid";
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
        LOGERROR() << infoLog;
        return false;
    }

    return true;
}

const UINT32 Shader::ConvertShaderType()
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
        LOGERROR() << "[Shader] Failed to open shader file, cause the file path is empty";
        return std::string();
    }

    LOGINFO() << "[Shader] " << m_FilePath << "";

    std::ifstream file(m_FilePath);
    if (!file.is_open())
    {
        LOGERROR() << "[Shader] Failed to open shader file";
        return std::string();
    }

    std::stringstream fileBuffer;
    fileBuffer << file.rdbuf();
    std::string shaderCode = fileBuffer.str();
    
    return shaderCode;
}