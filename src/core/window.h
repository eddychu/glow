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
  int multisample{0};
};

class Window {
public:
  Window(const WindowConfig &config);

  ~Window();
  void poll_events() const;
  void swap_buffers() const;
  bool should_close() const;

  int width() const { return m_width; }
  int height() const { return m_height; }

  void get_mouse_position(double &x, double &y) const {
    glfwGetCursorPos(m_window, &x, &y);
  }

  std::array<int, 2> get_size() const {
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return {width, height};
  }

  GLFWwindow *get_window() const { return m_window; }

private:
  GLFWwindow *m_window;
  int m_width{0};
  int m_height{0};
};