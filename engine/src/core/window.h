#pragma once
#include "pch.h"
#include "module.h"
#if defined(SDL2_WINDOW)
class SDL_Window;
#elif defined(GLFW_WINDOW)
class GLFWwindow;
#else
#endif

namespace ge
{
    struct WindowConfig
    {
        uint32_t width{800};
        uint32_t height{600};
        std::string title{"default"};
    };

    class Window : public IModule
    {
    public:
        void init(void *configuration) override;
        void shutdown() override;
        void pollEvents();
        GE_INLINE void* nativeHandle() {            
            return this->_window;
        };
    private:
#if defined(SDL2_WINDOW)
        SDL_Window *_window;
#elif defined(GLFW_WINDOW)
        GLFWwindow *_window;
#else
    void* _window{nullptr};
#endif
    };
}