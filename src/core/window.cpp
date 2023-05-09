#include <array>
#include <core/window.h>
#include <spdlog/spdlog.h>
void Window::initialize(const WindowConfig &config) {
  if (!glfwInit()) {
    spdlog::error("Failed to initialize GLFW");
    return;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(config.width, config.height, config.title,
                              nullptr, nullptr);

  if (!m_window) {
    spdlog::error("Failed to create GLFW window");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(m_window);
  glfwSetWindowUserPointer(m_window, this);
  glfwSetFramebufferSizeCallback(m_window, window_size_callback);
  glfwSetCursorPosCallback(m_window, cursor_pos_callback);
  glfwSetMouseButtonCallback(m_window, mouse_button_callback);
  glfwSetScrollCallback(m_window, scroll_callback);
  glfwSetKeyCallback(m_window, key_callback);
  // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSwapInterval(0);
  m_width = config.width;
  m_height = config.height;

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::error("Failed to initialize GLAD");
    return;
  }

  // spdlog::info("OpenGL Version: %s", glGetString(GL_VERSION)));

  spdlog::info("GLFW Version: %s", glfwGetVersionString());
}

void Window::poll_events() const { glfwPollEvents(); }

bool Window::should_close() const { return glfwWindowShouldClose(m_window); }

std::array<int, 2> Window::get_size() const { return {m_width, m_height}; }

void Window::destroy() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}