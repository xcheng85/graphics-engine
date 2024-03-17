#pragma once
#include <string>
#include <functional>

using logFunc = std::function<void(const std::string &&)>;

class IRequest
{
public:
    IRequest(int id, logFunc f) : _id{id}, _logFunc{f}
    {
    }
    virtual ~IRequest() = default;
    virtual void exec() noexcept = 0;

protected:
    int _id{-1};
    logFunc _logFunc;
};