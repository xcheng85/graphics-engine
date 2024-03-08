#pragma once
#include "core/event.h"

namespace ge
{
    // glfw event mapping to domain event
    struct QuitEvent : IEvent
    {
        GE_INLINE QuitEvent() = default;
    };
}