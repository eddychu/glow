#include <core/camera.h>

Camera::Camera(const CameraConfig &config) {
  m_target = config.target;
  m_fov = config.fov;
  m_near = config.near;
  m_far = config.far;
  glm::mat4 view = glm::lookAt(config.position, config.target, config.up);
  m_transform.set_matrix_invert(view);
  m_projection =
      glm::perspective(config.fov, config.aspect, config.near, config.far);
}