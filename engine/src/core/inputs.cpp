#include <GLFW/glfw3.h>
#include "pch.h"
#include "inputs.h"
#include "events/domain.h"

namespace ge::inputs
{
  static const uint8_t *keyboard = NULL;
  static MouseState mouse;
  static EventDispatcher ed;

  EventDispatcher *eventDispatcher() { return &ed; }
  static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
  {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      ed.addEvent<QuitEvent>();
      // glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  }

  void initialize(GLFWwindow *window)
  {
    glfwSetKeyCallback(window, key_callback);
  }

  // all the events in this frame will be disptach all at once
  // no conflict with rendering process
  void dispatchEvents()
  {
    ed.dispatch();
  }
}
