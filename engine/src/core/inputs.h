#pragma once
#include <glm/vec2.hpp>
#include "event-dispatcher.h"

class GLFWwindow;

namespace ge::inputs
{
    using namespace glm;
    struct MouseState
    {
        std::bitset<7> buttons = {0};
        vec2 offset;
        vec2 wheel;
    };
    // only declaration for hidden impl
    GE_API void initialize(GLFWwindow *);
    GE_API EventDispatcher *eventDispatcher();
    GE_API void dispatchEvents();
}