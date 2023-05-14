#include <rendering/skypass.h>
#include <geometry/geometry.h>
void SkyPass::init(GLProgram *program, TextureCube *texture, Camera *camera) {
  Geometry cube = make_cube(2.0f);
  geometry = new GeometryBuffer{cube.vertices, cube.indices};
  this->program = program;
  this->texture = texture;
  this->camera = camera;
}

void SkyPass::render() {
  program->use();
  program->set_uniform("view", GLUniform{mat4(mat3(camera->view_matrix()))});
  program->set_uniform("projection", GLUniform{camera->projection_matrix()});
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id());
  glBindVertexArray(geometry->handle());
  glDrawElements(GL_TRIANGLES, geometry->count(), GL_UNSIGNED_SHORT, nullptr);
}

void SkyPass::destroy() {
  delete geometry;
  geometry = nullptr;
  program = nullptr;
  texture = nullptr;
  camera = nullptr;
}

SkyPass::~SkyPass() { destroy(); }