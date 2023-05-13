#pragma once
#include <resource/resource.h>
#include <resource/texture.h>
#include <resource/geometry.h>
#include <resource/shader.h>
#include <core/camera.h>
#include <stdint.h>
struct SkyPass {
  GLProgram *program{nullptr};
  TextureCube *texture{nullptr};
  Camera *camera{nullptr};
  GLuint vbo{0};
  GLuint vao{0};
  GLuint ebo{0};
  uint32_t count{0};
  void init(GLProgram *program, TextureCube *texture, Camera *camera);

  void render();

  void destroy();

  ~SkyPass();
};