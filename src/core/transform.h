#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using namespace glm;
class Transform {

public:
  Transform() : m_position(0, 0, 0), m_rotation(0, 0, 0, 1), m_scale(1, 1, 1) {
    update_matrix();
  }
  Transform(const vec3 &position, const quat &rotation, const vec3 &scale)
      : m_position(position), m_rotation(rotation), m_scale(scale) {
    update_matrix();
  }
  void update_matrix() {
    m_matrix = translate(mat4(1), m_position) * mat4_cast(m_rotation) *
               glm::scale(mat4(1), m_scale);
    m_matrix_invert = inverse(m_matrix);
  }

  void set_position(const vec3 &position) {
    m_position = position;
    update_matrix();
  }

  void set_rotation(const quat &rotation) {
    m_rotation = rotation;
    update_matrix();
  }

  void set_scale(const vec3 &scale) {
    m_scale = scale;
    update_matrix();
  }

  void set_matrix(const mat4 &matrix) {
    m_matrix = matrix;
    m_matrix_invert = inverse(m_matrix);
    m_position = vec3(m_matrix[3]);
    m_rotation = quat_cast(m_matrix);
    m_scale =
        vec3(length(m_matrix[0]), length(m_matrix[1]), length(m_matrix[2]));
  }

  void set_matrix_invert(const mat4 &matrix_invert) {
    m_matrix_invert = matrix_invert;
    m_matrix = inverse(m_matrix_invert);
    m_position = vec3(m_matrix[3]);
    m_rotation = quat_cast(m_matrix);
    m_scale =
        vec3(length(m_matrix[0]), length(m_matrix[1]), length(m_matrix[2]));
  }

  const vec3 &position() const { return m_position; }
  const quat &rotation() const { return m_rotation; }
  const vec3 &scale() const { return m_scale; }

  const vec3 front() const { return m_matrix[2]; }
  const vec3 up() const { return m_matrix[1]; }
  const vec3 right() const { return m_matrix[0]; }

  const mat4 &matrix() const { return m_matrix; }

  const mat4 &matrix_invert() const { return m_matrix_invert; }

private:
  vec3 m_position;
  quat m_rotation;
  vec3 m_scale;
  mat4 m_matrix;
  mat4 m_matrix_invert;
};