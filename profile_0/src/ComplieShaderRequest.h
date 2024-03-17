#pragma once
#include <iostream>
#include "IRequest.h"

class CompileShaderRequest : public IRequest
{
public:
    explicit CompileShaderRequest(int id, logFunc f) : IRequest(id, f)
    {
    }

    void exec() noexcept override
    {
        _logFunc("CompileShaderRequest: exec()\n");
    }
};