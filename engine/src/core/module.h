#pragma once
#include "pch.h"

namespace ge
{
    class IModule
    {
    public:
        virtual void init(void *configuration) {}
        virtual void shutdown() {}
    }; // struct Service

#define GE_SINGLETON(Type) static Type *instance();
}