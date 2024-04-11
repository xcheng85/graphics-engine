#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>
#include "matrix.h"

using namespace std;

class Shader
{
public:
    Shader() = default;
    Shader(const std::string &filePath)
    {
        const auto shaderText = parseShaderFile(filePath);
        // 
        const auto shaderTextPtr = shaderText.c_str();
        const auto shaderType = parseShaderType(filePath);
        _shaderProgramHandle = glCreateShaderProgramv(shaderType, 1, &shaderTextPtr);
        checkCompileErrors(_shaderProgramHandle);
    }
    ~Shader()
    {
        // glDeleteProgram(_shaderProgramHandle);
    }

    inline auto programHandle() const
    {
        return _shaderProgramHandle;
    }

    // void setBool(const std::string &name, bool value) const
    // {
    //     glUniform1i(glGetUniformLocation(_shaderProgramId, name.c_str()), (int)value);
    // }

    // void setInt(const std::string &name, int value) const
    // {
    //     glUniform1i(glGetUniformLocation(_shaderProgramId, name.c_str()), value);
    // }

    // void setFloat(const std::string &name, float value) const
    // {
    //     glUniform1f(glGetUniformLocation(_shaderProgramId, name.c_str()), value);
    // }

    // void setVec2(const std::string &name, float x, float y) const
    // {
    //     glUniform2f(glGetUniformLocation(_shaderProgramId, name.c_str()), x, y);
    // }

    // void setVec3(const std::string &name, float x, float y, float z) const
    // {
    //     glUniform3f(glGetUniformLocation(_shaderProgramId, name.c_str()), x, y, z);
    // }

    // void setVec4(const std::string &name, float x, float y, float z, float w) const
    // {
    //     glUniform4f(glGetUniformLocation(_shaderProgramId, name.c_str()), x, y, z, w);
    // }

    // void setMat4(const std::string &name, const mat4x4f &mat) const
    // {
    //     // count 1,
    //     // no transpose
    //     glUniformMatrix4fv(glGetUniformLocation(_shaderProgramId, name.c_str()), 1, GL_FALSE, &mat.data[0][0]);
    // }

private:
    GLenum parseShaderType(const std::string &filePath)
    {
        if (filePath.ends_with(".vert"))
        {
            return GL_VERTEX_SHADER;
        }

        if (filePath.ends_with(".frag"))
        {
            return GL_FRAGMENT_SHADER;
        }

        if (filePath.ends_with(".geom"))
        {
            return GL_GEOMETRY_SHADER;
        }

        if (filePath.ends_with(".ctrl"))
        {
            return GL_TESS_CONTROL_SHADER;
        }

        if (filePath.ends_with(".eval"))
        {
            return GL_TESS_EVALUATION_SHADER;
        }

        if (filePath.ends_with(".comp"))
        {
            return GL_COMPUTE_SHADER;
        }
        assert(false);
        return 0;
    }

    // recursive due to unlimited #include possibility
    std::string parseShaderFile(const std::string &filePath)
    {
        std::string shaderText;
        // RAII
        std::ifstream fs{filePath};
        try
        {
            if (fs.fail())
            {
                throw std::ifstream::failure("fail to read shaderfile");
            }
            getline(fs, shaderText, '\0');
            // printShaderSource(shaderText.c_str());
            cout << shaderText << "\n";
            while (shaderText.find("#include ") != shaderText.npos)
            {
                const auto pos = shaderText.find("#include ");
                const auto p1 = shaderText.find('<', pos);
                const auto p2 = shaderText.find('>', pos);
                if (p1 == shaderText.npos || p2 == shaderText.npos || p2 <= p1)
                {
                    std::cerr << "error parsing #include\n";
                    assert(false);
                }
                // header name
                const std::string headerName = shaderText.substr(p1 + 1, p2 - p1 - 1);
                const std::string include = parseShaderFile(headerName);
                shaderText.replace(pos, p2 - pos + 1, include.c_str());
            }
        }
        catch (std::ifstream::failure &e)
        {
            std::cout << "Shader ctor: " << e.what() << std::endl;
        }
        return shaderText;
    }

    void checkCompileErrors(GLuint shaderHandle)
    {
        GLint success;
        GLchar infoLog[1024];

        glGetProgramiv(shaderHandle, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderHandle, 1024, NULL, infoLog);
            std::cerr << infoLog << "\n";
        }

        // else
        // {
        //     glGetProgramiv(shaderHandle, GL_LINK_STATUS, &success);
        //     if (!success)
        //     {
        //         glGetProgramInfoLog(shaderHandle, 1024, NULL, infoLog);
        //         std::cerr << "Error: " << type << "\n"
        //                   << infoLog << "\n";
        //     }
        // }
    }

    void printShaderSource(const char *shader)
    {
        int line{1};
        cout << format("\n{} ", line);
        while (shader && *shader++)
        {
            if (*shader == '\n')
            {
                cout << format("\n{} ", ++line);
            }
            else if (*shader == '\r')
            {
            }
            else
            {
                cout << format("{}", *shader);
            }
        }
        cout << "\n";
    }

    GLuint _shaderProgramHandle;
};
