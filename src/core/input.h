#pragma once
#include <core/window.h>
class Input {
public:
  static void initialize(Window *window) {
    Input::m_window = window->get_window();
  }
  static bool is_key_pressed(int keycode) {
    return glfwGetKey(Input::m_window, keycode) == GLFW_PRESS;
  }
  static bool is_mouse_button_pressed(int button) {
    return glfwGetMouseButton(Input::m_window, button) == GLFW_PRESS;
  }

private:
  static GLFWwindow *m_window;
};
