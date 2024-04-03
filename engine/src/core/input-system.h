#pragma once
#include <glm/vec2.hpp>
#include "module.h"
#include "event-dispatcher.h"

namespace ge
{
    // using namespace glm;
    // struct MouseState
    // {
    //     std::bitset<7> buttons = {0};
    //     vec2 offset;
    //     vec2 wheel;
    // };
    // // only declaration for hidden impl
    // GE_API void initialize(GLFWwindow *);
    // GE_API EventDispatcher *eventDispatcher();
    // GE_API void dispatchEvents();

    class InputSystem : public IModule
    {
    public:
        GE_SINGLETON(InputSystem)
        void init(void *configuration) override;
        void shutdown() override;
        // EventDispatcher contain unique_ptr, use raw pointer
        GE_INLINE EventDispatcher* eventDispatcher() {
            return &_ed;
        }

        GE_API void dispatchEvents();

    private:
        EventDispatcher _ed;
    };
}