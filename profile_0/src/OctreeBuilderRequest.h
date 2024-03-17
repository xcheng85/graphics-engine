#pragma once
#include <iostream>
#include "IRequest.h"

class OctreeBuilderRequest : public IRequest
{
public:
    explicit OctreeBuilderRequest(int id, logFunc f) : IRequest(id, f)
    {
    }

    void exec() noexcept override
    {
        _logFunc("OctreeBuilderRequest: exec()\n");
    }
};