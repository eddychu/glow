#pragma once
#include <core/camera.h>
#include <core/window.h>
#include <spdlog/spdlog.h>
class CameraController {
public:
  CameraController(Camera *camera, Window *window)
      : camera(camera), window(window) {
    window->register_on_mouse_button_func(
        [&](int button, int action, int mods) {
          spdlog::info("mouse button");
          if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            enabled = true;
            spdlog::info("enabled");
            last_cursor_pos = window->get_cursor_pos();
          }
          if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            enabled = false;
          }
        });

    window->register_on_cursor_pos_func([&](double x, double y) {
      // spdlog::info("cursor pos");
      if (!enabled)
        return;

      double dx = x - last_cursor_pos[0];
      double dy = y - last_cursor_pos[1];
      rotate(dx, dy);
      last_cursor_pos = {x, y};
    });
  }
  void set_enable(bool enable) { enabled = enable; }

  void rotate(float dx, float dy) {
    if (!enabled)
      return;
    vec3 dist_vec = camera->transform().position() - camera->target();
    float radius = glm::length(dist_vec);
    float theta = 0.0;
    float phi = 0.0;
    if (radius > 0.001) {
      theta = atan2(dist_vec.x, dist_vec.z);
      float v = min(1.0f, max(-1.0f, dist_vec.y / radius));
      phi = acos(v);
    }
    auto size = window->get_size();
    int span = std::min(size[0], size[1]);
    theta -= dx * glm::pi<float>() * 2.0 / span;
    phi -= dy * glm::pi<float>() * 2.0 / span;

    if (phi < 0.001) {
      phi = 0.001;
    } else if (phi > glm::pi<float>() - 0.001) {
      phi = glm::pi<float>() - 0.001;
    }

    float sin_phi = sin(phi) * radius;
    float new_x = sin_phi * sin(theta);
    float new_z = sin_phi * cos(theta);
    float new_y = cos(phi) * radius;
    vec3 eye = vec3(new_x, new_y, new_z);
    camera->set_position(eye);
    // view_matrix = glm::lookAt(eye, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
  }

private:
  Camera *camera{nullptr};
  bool enabled{false};
  Window *window{nullptr};
  std::array<double, 2> last_cursor_pos{0.0, 0.0};
};