#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// reuse shader program
class Pipeline
{
public:
    Pipeline() = default;
    explicit Pipeline(const unordered_map<GLenum, GLuint> &shaderProgramMap)
    {
        glCreateProgramPipelines(1, &_pipelineHandle);
        for (const auto &[shaderType, shaderProgramStage] : shaderProgramMap)
        {
            glUseProgramStages(_pipelineHandle, shaderType, shaderProgramStage);
        }
    }

    Pipeline &operator=(const Pipeline &other)
    {
        _pipelineHandle = other._pipelineHandle;
        return *this;
    }

    Pipeline &operator=(Pipeline &&other) noexcept
    {
        _pipelineHandle = other._pipelineHandle;
        return *this;
    }

    ~Pipeline()
    {
        glDeleteProgramPipelines(1, &_pipelineHandle);
    }

    inline GLuint handle()
    {
        return _pipelineHandle;
    }

    void bind()
    {
        return glBindProgramPipeline(_pipelineHandle);
    }

private:
    GLuint _pipelineHandle;
};
