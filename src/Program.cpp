#include "Program.h"

#include <LogProject/Log.h>

#include "glad/glad.h"

Program::Program()
	: m_ProgramID(GL_NONE)
	, m_umShader{}
{}

Program::~Program()
{
	if (m_ProgramID != GL_NONE)
	{
		for (const auto& shaderPair : m_umShader)
		{
			const auto& shader = shaderPair.second;

			DetachShader(shader);
		}

		LOGINFO() << "[Program] Delete Program(" << m_ProgramID <<")";

		glDeleteProgram(m_ProgramID);

		m_ProgramID = GL_NONE;
	}
}

bool Program::SetShader(const Shader::Type& _shaderType, const std::string& _shaderPath)
{
    Shader shader(_shaderType, _shaderPath);

    if (!shader.Init())
    {
        LOGERROR() << "[Program] Failed to init Shader(" << static_cast<UINT8>(_shaderType) << "[Program] ) Path(" << _shaderPath <<")";
        return false;
    }

	const auto itrShader = m_umShader.find(_shaderType);
	if (itrShader != m_umShader.end())
	{
		const auto originShader = itrShader->second;
		DetachShader(originShader);
	}

	Create();
	AttachShader(shader);

	m_umShader.insert(std::make_pair(_shaderType, shader));

	return true;
}

void Program::AttachShader(const Shader& _shader)
{
    if (m_ProgramID == GL_NONE)
    {
        LOGERROR() << "[Program] Failed to attach shader, the program is invalid";
        return;
    }

	glAttachShader(m_ProgramID, _shader.GetShaderID());

    if (!Link())
	{
		LOGERROR() << "[Program] Failed to attach shader(" << _shader.GetShaderID() << "[Program] )";
	}
}

void Program::DetachShader(const Shader& _shader)
{
	if (m_ProgramID == GL_NONE)
	{
		LOGERROR() << "[Program] Failed to detach shader, the program is invalid";
		return;
	}

	if (CheckLinked())
	{
		glDetachShader(m_ProgramID, _shader.GetShaderID());
	}
}

void Program::Create()
{
	if (m_ProgramID == GL_NONE)
	{
		m_ProgramID = glCreateProgram();
	}
	else
	{
		LOGWARN() << "[Program] The gl program is already made";
	}
}

bool Program::Link()
{
	char cInfoLog[512];
	glLinkProgram(m_ProgramID);

	if (!CheckLinked())
	{
		glGetProgramInfoLog(m_ProgramID, 512, nullptr, cInfoLog);
		LOGINFO() << "[Program] Failed to link program :" << cInfoLog;

		return false;
	}

	return true;
}

bool Program::CheckLinked()
{
    GLint nLinkStatus = GL_NONE;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &nLinkStatus);

    if (nLinkStatus == GL_TRUE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

const UINT64 Program::GetProgramID()
{
	return m_ProgramID;
}