#pragma once

#include <resource/texture.h>
#include <resource/shader.h>
#include <stdint.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
struct PrefilterPass {

  // TextureCube *texture{nullptr};
  uint32_t mip_levels{5};

  GLProgram *program{nullptr};
  TextureCube *environment_texture{nullptr};

  int width{128};
  int height{128};

  GLuint capture_fbo{0};
  GLuint capture_rbo{0};

  GLuint prefilter_texture{0};

  mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

  std::vector<mat4> views = {
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, -1.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                  glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, -1.0f, 0.0f))};

  GLuint vbo{0};
  GLuint vao{0};
  GLuint ebo{0};
  uint32_t count{0};

  void init(GLProgram *program, TextureCube *environment_texture);

  void render();
};