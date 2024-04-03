#include "pch.h"
#include "input-system.h"

namespace ge
{
  // static const uint8_t *keyboard = NULL;
  // static MouseState mouse;

  static InputSystem sInputSystem;

  InputSystem *InputSystem::instance()
  {
    return &sInputSystem;
  }

  // static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
  // {
  //   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  //   {
  //     ed.addEvent<QuitEvent>();
  //     // glfwSetWindowShouldClose(window, GLFW_TRUE);
  //   }
  // }

  void InputSystem::init(void *configPtr)
  {
    // glfwSetKeyCallback(window, key_callback);
  }

  void InputSystem::shutdown() {
    
  }

  // all the events in this frame will be disptach all at once
  // no conflict with rendering process
  void InputSystem::dispatchEvents()
  {
    _ed.dispatch();
  }
}
