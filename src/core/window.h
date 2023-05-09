#pragma once
#include <array>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
struct WindowConfig {
  int width{1280};
  int height{720};
  const char *title{"glow 0.1"};
  bool is_fullscreen{false};
};

class Window {
public:
  Window() = default;

  void initialize(const WindowConfig &config);
  void poll_events() const;
  std::array<int, 2> get_size() const;
  bool should_close() const;
  GLFWwindow *window() const { return m_window; }
  void set_should_close(bool should_close) const {
    glfwSetWindowShouldClose(m_window, should_close);
  }
  void destroy();

  typedef std::function<void(int, int, int, int)> onKeyFunc;
  typedef std::function<void(unsigned int)> onCharFunc;
  typedef std::function<void(int, unsigned int)> onCharModsFunc;
  typedef std::function<void(int, int, int)> onMouseButtonFunc;
  typedef std::function<void(double, double)> onCursorPosFunc;
  typedef std::function<void(int)> onCursorEnterFunc;
  typedef std::function<void(double, double)> onScrollFunc;
  typedef std::function<void(int, const char **)> onDropFunc;
  typedef std::function<void(int, int)> onWindowSizeFunc;
  typedef std::function<void()> onWindowCloseFunc;

  void register_on_key_func(onKeyFunc func) { m_on_key_funcs.push_back(func); }
  void register_on_char_func(onCharFunc func) {
    m_on_char_funcs.push_back(func);
  }
  void register_on_char_mods_func(onCharModsFunc func) {
    m_on_char_mods_funcs.push_back(func);
  }
  void register_on_mouse_button_func(onMouseButtonFunc func) {
    m_on_mouse_button_funcs.push_back(func);
  }
  void register_on_cursor_pos_func(onCursorPosFunc func) {
    m_on_cursor_pos_funcs.push_back(func);
  }
  void register_on_cursor_enter_func(onCursorEnterFunc func) {
    m_on_cursor_enter_funcs.push_back(func);
  }
  void register_on_scroll_func(onScrollFunc func) {
    m_on_scroll_funcs.push_back(func);
  }
  // void register_on_drop_func(onDropFunc func);
  void register_on_window_size_func(onWindowSizeFunc func) {
    m_on_window_size_funcs.push_back(func);
  }
  // void register_on_window_close_func(onWindowCloseFunc func) {
  //   m_on_window_close_funcs.push_back(func);
  // }

  bool is_mouse_button_down(int button) const {
    if (button < GLFW_MOUSE_BUTTON_1 || button > GLFW_MOUSE_BUTTON_LAST)
      return false;
    return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
  }

protected:
  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods) {
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self) {
      self->on_key(key, scancode, action, mods);
    }
  }

  static void char_callback(GLFWwindow *window, unsigned int codepoint) {
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self) {
      self->on_char(codepoint);
    }
  }

  static void char_mods_callback(GLFWwindow *window, unsigned int codepoint,
                                 int mods) {
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self) {
      self->on_char_mods(codepoint, mods);
    }
  }

  static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                    int mods) {
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self) {
      self->on_mouse_button(button, action, mods);
    }
  }

  static void cursor_pos_callback(GLFWwindow *window, double xpos,
                                  double ypos) {
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self) {
      self->on_cursor_pos(xpos, ypos);
    }
  }

  static void cursor_enter_callback(GLFWwindow *window, int entered) {
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self) {
      self->on_cursor_enter(entered);
    }
  }

  static void scroll_callback(GLFWwindow *window, double xoffset,
                              double yoffset) {
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self) {
      self->on_scroll(xoffset, yoffset);
    }
  }

  static void window_size_callback(GLFWwindow *window, int width, int height) {
    auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self) {
      self->on_window_size(width, height);
    }
  }

  static void window_close_callback(GLFWwindow *window) {
    glfwSetWindowShouldClose(window, true);
  }

  void on_key(int key, int scancode, int action, int mods) {
    for (auto &func : m_on_key_funcs) {
      func(key, scancode, action, mods);
    }
  }

  void on_char(unsigned int codepoint) {
    for (auto &func : m_on_char_funcs) {
      func(codepoint);
    }
  }

  void on_char_mods(int codepoint, int mods) {
    for (auto &func : m_on_char_mods_funcs) {
      func(codepoint, mods);
    }
  }

  void on_mouse_button(int button, int action, int mods) {
    for (auto &func : m_on_mouse_button_funcs) {
      func(button, action, mods);
    }
  }

  void on_cursor_pos(double xpos, double ypos) {
    for (auto &func : m_on_cursor_pos_funcs) {
      func(xpos, ypos);
    }
  }

  void on_cursor_enter(int entered) {
    for (auto &func : m_on_cursor_enter_funcs) {
      func(entered);
    }
  }

  void on_scroll(double xoffset, double yoffset) {
    for (auto &func : m_on_scroll_funcs) {
      func(xoffset, yoffset);
    }
  }

  void on_window_size(int width, int height) {
    for (auto &func : m_on_window_size_funcs) {
      func(width, height);
    }
  }

private:
  GLFWwindow *m_window;
  int m_width{0};
  int m_height{0};

  std::vector<onKeyFunc> m_on_key_funcs;
  std::vector<onCharFunc> m_on_char_funcs;
  std::vector<onCharModsFunc> m_on_char_mods_funcs;
  std::vector<onMouseButtonFunc> m_on_mouse_button_funcs;
  std::vector<onCursorPosFunc> m_on_cursor_pos_funcs;
  std::vector<onCursorEnterFunc> m_on_cursor_enter_funcs;
  std::vector<onScrollFunc> m_on_scroll_funcs;
  // std::vector<onDropFunc> m_on_drop_funcs;
  std::vector<onWindowSizeFunc> m_on_window_size_funcs;
};