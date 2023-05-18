#pragma once

#include <cmath>
#include <core/transform.h>
#include <spdlog/spdlog.h>
struct CameraConfig {
  vec3 position = vec3(0.0, 0.0, 5.0);
  vec3 target = vec3(0.0, 0.0, 0.0);
  vec3 up = vec3(0.0, 1.0, 0.0);
  float fov = glm::radians(45.0);
  float aspect = 1.0;
  float near = 0.1;
  float far = 1000.0;
};

class Camera {
public:
  Camera(const CameraConfig &config = CameraConfig());
  mat4 view_matrix() const { return m_transform.matrix_invert(); }
  mat4 projection_matrix() const { return m_projection; }
  void set_position(const glm::vec3 &position) {
    spdlog::info("set position");
    glm::mat4 view = glm::lookAt(position, m_target, vec3(0.0, 1.0, 0.0));
    m_transform.set_matrix_invert(view);
  }
  void set_rotation(const glm::quat &rotation) {
    m_transform.set_rotation(rotation);
  }
  void set_target(const glm::vec3 &target) {
    m_target = target;
    glm::mat4 view =
        glm::lookAt(m_transform.position(), m_target, m_transform.up());
    m_transform.set_matrix_invert(view);
  }

  void set_aspect(float aspect) {
    m_aspect = aspect;
    m_projection = glm::perspective(m_fov, aspect, m_near, m_far);
  }

  vec3 target() const { return m_target; }

  const Transform &transform() const { return m_transform; }

private:
  Transform m_transform;
  vec3 m_target;
  mat4 m_projection;
  float m_fov;
  float m_near;
  float m_far;
  float m_aspect;
};