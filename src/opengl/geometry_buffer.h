#pragma once
#include <memory>
#include <opengl/resource.h>
#include <scene/geometry.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stdint.h>
#include <vector>
#include <glad/glad.h>
using namespace glm;

class GeometryBuffer : public Resource {
public:
  GeometryBuffer(const Geometry &geometry);

  void destroy();

  ~GeometryBuffer();

  bool has_indices() const { return m_has_indices; }
  GLuint handle() const { return vao; }
  GLuint count() const { return m_count; }

  void draw() const {
    glBindVertexArray(vao);
    if (m_has_indices) {
      glDrawElements(m_mode, m_count, GL_UNSIGNED_INT, 0);
    } else {
      glDrawArrays(m_mode, 0, m_count);
    }
  }

private:
  void setup_vbo(const std::vector<Vertex> &vertices);
  GLuint vao{0};
  GLuint vbo{0};
  GLuint ebo{0};
  bool m_has_indices{false};
  GLuint m_count{0};
  GLenum m_mode{GL_TRIANGLES};
};