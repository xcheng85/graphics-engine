#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "pch.h"
#include "core/inputs.h"
#include "events/domain.h"
#include "application.h"

namespace ge
{
    // singleton
    static AppConfig config;
    static bool _running = true;

    static void error_callback(int error, const char *description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }
    GE_INLINE bool onQuit(const QuitEvent &)
    {
        return _running = false;
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

        while (_running)
        {
            glfwSwapBuffers(window);
            glfwPollEvents();
            inputs::dispatchEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}