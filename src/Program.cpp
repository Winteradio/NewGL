#include "Program.h"

#include "glad/glad.h"
#include "NewLog.h"

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

		LOG(INFO) << "Delete Program(" << m_ProgramID <<")";

		glDeleteProgram(m_ProgramID);

		m_ProgramID = GL_NONE;
	}
}

bool Program::SetShader(const Shader::Type& _shaderType, const std::string& _shaderPath)
{
    Shader shader(_shaderType, _shaderPath);

    if (!shader.Init())
    {
        LOG(ERROR) << "Failed to init Shader(" << static_cast<UINT8>(_shaderType) << ") Path(" << _shaderPath <<")";
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
        LOG(ERROR) << "Failed to attach shader, the program is invalid\n";
        return;
    }

	glAttachShader(m_ProgramID, _shader.GetShaderID());

    if (!Link())
	{
		LOG(ERROR) << "Failed to attach shader(" << _shader.GetShaderID() << ")\n";
	}
}

void Program::DetachShader(const Shader& _shader)
{
	if (m_ProgramID == GL_NONE)
	{
		LOG(ERROR) << "Failed to detach shader, the program is invalid\n";
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
		LOG(WARN) << "The gl program is already made\n";
	}
}

bool Program::Link()
{
	char cInfoLog[512];
	glLinkProgram(m_ProgramID);

	if (!CheckLinked())
	{
		glGetProgramInfoLog(m_ProgramID, 512, nullptr, cInfoLog);
		LOG(INFO) << "Failed to link program :" << cInfoLog << "\n";

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