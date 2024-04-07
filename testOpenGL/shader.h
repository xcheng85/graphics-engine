#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "matrix.h"
using namespace std;
class Shader
{
    enum ShaderObjectType : int
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        SHADER_PROGRAM
    };

public:
    Shader() = default;
    Shader(const char *vsPath, const char *fsPath)
    {
        std::string vs;
        std::string fs;

        // RAII
        std::ifstream vsFile{vsPath};
        std::ifstream fsFile{fsPath};

        try
        {
            if (vsFile.fail() || fsFile.fail())
            {
                throw std::ifstream::failure("fail to read shaderfile");
            }
            getline(vsFile, vs, '\0');
            getline(fsFile, fs, '\0');

            cout << vs << "\n";
            cout << fs << "\n";
        }
        catch (std::ifstream::failure &e)
        {
            std::cout << "Shader ctor: " << e.what() << std::endl;
        }

        unsigned int vsHandle, fsHandle;
        auto vsPtr = vs.c_str(), fsPtr = fs.c_str();

        vsHandle = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vsHandle, 1, &vsPtr, NULL);
        glCompileShader(vsHandle);
        checkCompileErrors(vsHandle, ShaderObjectType::VERTEX_SHADER);

        fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsHandle, 1, &fsPtr, NULL);
        glCompileShader(fsHandle);
        checkCompileErrors(fsHandle, ShaderObjectType::FRAGMENT_SHADER);

        _shaderProgramId = glCreateProgram();
        glAttachShader(_shaderProgramId, vsHandle);
        glAttachShader(_shaderProgramId, fsHandle);
        glLinkProgram(_shaderProgramId);
        checkCompileErrors(_shaderProgramId, ShaderObjectType::SHADER_PROGRAM);

        glDeleteShader(vsHandle);
        glDeleteShader(fsHandle);
    }

    void activate() const
    {
        glUseProgram(_shaderProgramId);
    }

    inline auto programHandle() const {
        return _shaderProgramId;
    }

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(_shaderProgramId, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(_shaderProgramId, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(_shaderProgramId, name.c_str()), value);
    }

    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(_shaderProgramId, name.c_str()), x, y);
    }

    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(_shaderProgramId, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(_shaderProgramId, name.c_str()), x, y, z, w);
    }

    void setMat4(const std::string &name, const mat4x4f &mat) const
    {
        // count 1, 
        // no transpose
        glUniformMatrix4fv(glGetUniformLocation(_shaderProgramId, name.c_str()), 1, GL_FALSE, &mat.data[0][0]);
    }

private:
    void checkCompileErrors(GLuint shaderHandle, ShaderObjectType type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != ShaderObjectType::SHADER_PROGRAM)
        {
            glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shaderHandle, 1024, NULL, infoLog);
                std::cerr << "Error: " << type << "\n"
                          << infoLog << "\n";
            }
        }
        else
        {
            glGetProgramiv(shaderHandle, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shaderHandle, 1024, NULL, infoLog);
                std::cerr << "Error: " << type << "\n"
                          << infoLog << "\n";
            }
        }
    }

    GLuint _shaderProgramId;
};
