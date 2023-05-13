#include <resource/geometry.h>
#include <spdlog/spdlog.h>
Geometry::Geometry(const std::vector<Vertex> &vertices)
    : Resource(ResourceType::Geometry) {
  m_has_indices = false;
  glCreateBuffers(1, &vbo);
  glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex), vertices.data(),
                       0);
  glCreateVertexArrays(1, &vao);
  glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
  setup_vbo(vertices);
  m_count = vertices.size();
}

Geometry::Geometry(const std::vector<Vertex> &vertices,
                   const std::vector<uint16_t> &indices)
    : Resource(ResourceType::Geometry) {
  m_has_indices = true;
  glCreateBuffers(1, &vbo);
  glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex), vertices.data(),
                       0);
  glCreateBuffers(1, &ebo);
  glNamedBufferStorage(ebo, indices.size() * sizeof(uint16_t), indices.data(),
                       0);
  glCreateVertexArrays(1, &vao);
  glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
  glVertexArrayElementBuffer(vao, ebo);
  setup_vbo(vertices);
  m_count = indices.size();
}

void Geometry::setup_vbo(const std::vector<Vertex> &vertices) {

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

void Geometry::destroy() {

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

Geometry::~Geometry() {
  spdlog::info("Geometry destroyed");
  destroy();
}