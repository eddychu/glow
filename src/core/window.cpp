#include <array>
#include <core/window.h>
#include <spdlog/spdlog.h>
Window::Window(const WindowConfig &config) {
  if (!glfwInit()) {
    spdlog::error("Failed to initialize GLFW");
    return;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, config.multisample);

  m_window = glfwCreateWindow(config.width, config.height, config.title,
                              nullptr, nullptr);

  if (!m_window) {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(m_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("Failed to initialize GLAD");
    return;
  }

  // glfwSetWindowUserPointer(m_window, this);

  // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSwapInterval(0);
  m_width = config.width;
  m_height = config.height;

  // spdlog::info("OpenGL Version: %s", glGetString(GL_VERSION)));

  spdlog::info("GLFW Version: %s", glfwGetVersionString());
  glEnable(GL_MULTISAMPLE);
}

void Window::poll_events() const { glfwPollEvents(); }

void Window::swap_buffers() const { glfwSwapBuffers(m_window); }

bool Window::should_close() const { return glfwWindowShouldClose(m_window); }

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}