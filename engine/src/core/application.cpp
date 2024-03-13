#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "pch.h"
#include "core/inputs.h"
#include "events/domain.h"
#include "application.h"
#include "ecs/scene_manager.h"

namespace ge
{
    // singleton
    // internal link
    static AppConfig config;
    static bool _running{true};
    static ecs::SceneManager *sm{nullptr};
    static double dt, lastTick;

    // stack-based memory allocator
    // pre-allocated memory block
    // life-cyle is per-frame
    std::array<std::byte, 1024> buf;
    std::pmr::monotonic_buffer_resource singleFrameAllocator{buf.data(), buf.size(),
                                                             std::pmr::null_memory_resource()};

    // // life-cyle is entire app
    // std::array<std::byte, 1024> buf2;
    // std::pmr::monotonic_buffer_resource ge::appAllocator{buf2.data(), buf2.size(),
    //                                                  std::pmr::null_memory_resource()};

    static void error_callback(int error, const char *description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }
    GE_INLINE bool onQuit(const QuitEvent &)
    {
        return _running = false;
    }
    GE_INLINE void updateDt()
    {
        lastTick = glfwGetTime();
        dt = glfwGetTime() - lastTick;
    }
    void runEventLoop(const AppConfig &cfg)
    {
        config = cfg;
        GLFWwindow *window;
        glfwSetErrorCallback(error_callback);

        if (!glfwInit())
        {
            GE_ERROR("glfwInit");
            exit(EXIT_FAILURE);
        }

        window = glfwCreateWindow(config.width, config.height, "Simple example", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(1);

        inputs::initialize(window);
        auto ed = inputs::eventDispatcher();
        ed->subscribe<QuitEvent>(onQuit);

        void *const p = appAllocator.allocate(sizeof(ecs::SceneManager), alignof(ecs::SceneManager));
        sm = new (p) ecs::SceneManager();
        sm->start();

        lastTick = glfwGetTime();
        while (_running)
        {
            // singleFrameAllocator.release();
            updateDt();
            sm->update(dt);

            glfwSwapBuffers(window);
            glfwPollEvents();
            inputs::dispatchEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}