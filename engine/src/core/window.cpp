#define GLFW_INCLUDE_NONE
#include <iostream>
#include <format>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_vulkan.h>
#include "window.h"
#include "input-system.h"
#include "events/domain.h"

namespace ge
{
    using namespace std;
    static void error_callback(int error, const char *description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }
    void Window::init(void *configPtr)
    {
        const auto &config = *(static_cast<WindowConfig *>(configPtr));
#if defined(SDL2_WINDOW)
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            cerr << format("SDL Init error: {}\n", SDL_GetError());
            return;
        }

        SDL_DisplayMode current;
        SDL_GetCurrentDisplayMode(0, &current);

        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        _window = SDL_CreateWindow(config.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width, config.height, window_flags);
        int w, h;
        SDL_Vulkan_GetDrawableSize(_window, &w, &h);

#elif defined(GLFW_WINDOW)
        if (!glfwInit())
        {
            GE_ERROR("glfwInit");
            exit(EXIT_FAILURE);
        }
        glfwSetErrorCallback(error_callback);
        _window = glfwCreateWindow(config.width, config.height, "Simple example", NULL, NULL);
        if (!_window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(_window);
        gladLoadGL();
        glfwSwapInterval(1);
#else
        cout << "no window\n";
#endif
    }

    void Window::shutdown()
    {
#if defined(SDL2_WINDOW)
        SDL_DestroyWindow(_window);
        SDL_Quit();
#elif defined(GLFW_WINDOW)
        glfwDestroyWindow(_window);
        glfwTerminate();
#else

#endif
    }

    void Window::pollEvents()
    {
#if defined(SDL2_WINDOW)
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                InputSystem::instance()->eventDispatcher()->addEvent<QuitEvent>();
                break;
            }
        }
#elif defined(GLFW_WINDOW)
        // only valid for opengl
        // glfwSwapBuffers(_window);
        glfwPollEvents();
#else

#endif
    }
}