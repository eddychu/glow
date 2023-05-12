#pragma once

#include <core/camera.h>

class CameraController {
public:
  CameraController(Camera *camera, int width, int height)
      : camera(camera), width(width), height(height) {}
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
    int span = std::min(width, height);
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
  int width{0};
  int height{0};
};