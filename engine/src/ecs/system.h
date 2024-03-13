#pragma once
#include "../pch.h"
#include "./ec_manager.h"

namespace ge::ecs
{
    struct ISystem
    {
        GE_INLINE virtual ~ISystem() = default;
        GE_INLINE void startup(EntityComponentManager *m)
        {
            this->_ecManager = m;
        }
        GE_INLINE virtual void update(float) {}
        GE_INLINE virtual void start() {}

    protected:
        EntityComponentManager *_ecManager{nullptr};
    };
}