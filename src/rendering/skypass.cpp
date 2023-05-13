#include <rendering/skypass.h>

void SkyPass::init(GLProgram *program, TextureCube *texture, Camera *camera) {
  std::vector<vec3> positions = {
      // Front
      vec3(-1.0f, -1.0f, 1.0f), vec3(1.0f, -1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f),
      vec3(-1.0f, 1.0f, 1.0f),
      // Right
      vec3(1.0f, -1.0f, 1.0f), vec3(1.0f, -1.0f, -1.0f),
      vec3(1.0f, 1.0f, -1.0f), vec3(1.0f, 1.0f, 1.0f),
      // Back
      vec3(-1.0f, -1.0f, -1.0f), vec3(-1.0f, 1.0f, -1.0f),
      vec3(1.0f, 1.0f, -1.0f), vec3(1.0f, -1.0f, -1.0f),
      // Left
      vec3(-1.0f, -1.0f, 1.0f), vec3(-1.0f, 1.0f, 1.0f),
      vec3(-1.0f, 1.0f, -1.0f), vec3(-1.0f, -1.0f, -1.0f),
      // Bottom
      vec3(-1.0f, -1.0f, 1.0f), vec3(-1.0f, -1.0f, -1.0f),
      vec3(1.0f, -1.0f, -1.0f), vec3(1.0f, -1.0f, 1.0f),
      // Top
      vec3(-1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, -1.0f),
      vec3(-1.0f, 1.0f, -1.0f)};
  std::vector<uint16_t> el = {0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,
                              8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,
                              16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};

  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &ebo);
  glCreateBuffers(1, &vbo);
  glNamedBufferStorage(vbo, positions.size() * sizeof(vec3), positions.data(),
                       0);
  glNamedBufferStorage(ebo, el.size() * sizeof(uint16_t), el.data(), 0);
  glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(vec3));
  glVertexArrayElementBuffer(vao, ebo);
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
  count = el.size();
  this->program = program;
  this->texture = texture;
  this->camera = camera;
}

void SkyPass::render() {
  program->use();
  program->set_uniform("view", GLUniform{mat4(mat3(camera->view_matrix()))});
  program->set_uniform("projection", GLUniform{camera->projection_matrix()});
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture->handle());
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
}

void SkyPass::destroy() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &vbo);
}

SkyPass::~SkyPass() { destroy(); }