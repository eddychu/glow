
#include "scene/geometry.h"
#include <opengl/geometry_buffer.h>
#include <spdlog/spdlog.h>
#include <stdint.h>

GeometryBuffer::GeometryBuffer(const Geometry &geometry) {
  set_id(geometry.id());
  m_has_indices = true;
  if (geometry.indices.size() == 0) {
    m_has_indices = false;
  }
  auto &vertices = geometry.vertices;
  auto &indices = geometry.indices;
  glCreateBuffers(1, &vbo);
  glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex), vertices.data(),
                       0);

  glCreateVertexArrays(1, &vao);
  glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
  setup_vbo(vertices);
  m_count = vertices.size();
  if (m_has_indices) {
    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, indices.size() * sizeof(uint32_t), indices.data(),
                         0);
    glVertexArrayElementBuffer(vao, ebo);
    m_count = indices.size();
  }

  if (geometry.mode == GeometryMode::Lines) {
    m_mode = GL_LINES;
  } else if (geometry.mode == GeometryMode::Points) {
    m_mode = GL_POINTS;
  }
}

void GeometryBuffer::setup_vbo(const std::vector<Vertex> &vertices) {

  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, position));
  glVertexArrayAttribBinding(vao, 0, 0);

  glEnableVertexArrayAttrib(vao, 1);
  glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, normal));
  glVertexArrayAttribBinding(vao, 1, 0);

  glEnableVertexArrayAttrib(vao, 2);
  glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, uv));
  glVertexArrayAttribBinding(vao, 2, 0);

  glEnableVertexArrayAttrib(vao, 3);
  glVertexArrayAttribFormat(vao, 3, 3, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, tangent));
  glVertexArrayAttribBinding(vao, 3, 0);

  glEnableVertexArrayAttrib(vao, 4);
  glVertexArrayAttribFormat(vao, 4, 3, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, bitangent));
  glVertexArrayAttribBinding(vao, 4, 0);
}

void GeometryBuffer::destroy() {

  if (vao) {
    glDeleteVertexArrays(1, &vao);
  }
  if (vbo) {
    glDeleteBuffers(1, &vbo);
  }
  if (ebo) {
    glDeleteBuffers(1, &ebo);
  }
}

GeometryBuffer::~GeometryBuffer() {
  spdlog::info("Geometry destroyed");
  destroy();
}