#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include "pch.h"
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

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
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

        glfwSetKeyCallback(window, key_callback);

        glfwMakeContextCurrent(window);
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(1);

        while (_running)
        {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
}