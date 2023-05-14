#pragma once
#include <resource/resource.h>
#include <resource/texture.h>
#include <resource/geometry_buffer.h>
#include <resource/shader.h>
#include <core/camera.h>
#include <stdint.h>
struct SkyPass {
  GLProgram *program{nullptr};
  TextureCube *texture{nullptr};
  Camera *camera{nullptr};
  GeometryBuffer *geometry{nullptr};
  void init(GLProgram *program, TextureCube *texture, Camera *camera);

  void render();

  void destroy();

  ~SkyPass();
};