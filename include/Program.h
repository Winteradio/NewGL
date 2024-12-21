#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "Shader.h"
#include <map>

class Program
{
    public :
        Program();
        virtual ~Program();

    public :
        bool SetShader(const Shader::Type& _shaderType, const std::string& _shaderPath);
        const UINT64 GetProgramID();

    private :
        void Create();
        bool Link();

        void AttachShader(const Shader& _shader);
        void DetachShader(const Shader& _shader);
        bool CheckLinked();

    private :
        UINT64 m_ProgramID;
        std::map<Shader::Type, Shader, Shader::TypeCompare> m_umShader;
};

#endif // __PROGRAM_H__