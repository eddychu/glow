#pragma once
#include <opengl/resource.h>
#include <glad/glad.h>
#include <variant>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include <scene/material.h>
using namespace glm;

struct GLShader {
  GLuint handle;
  GLShader(const char *path, GLenum type);
  void destroy() const;
  ~GLShader() {
    spdlog::info("GLShader destroyed");
    destroy();
  }
};

class GLProgram : public Resource {
public:
  GLProgram(const Material &mat);
  void populate_uniforms();
  void set_uniform(const char *name, const UniformValue &uniform) const;

  void use() const;
  void unuse() const;
  void destroy() const;
  ~GLProgram() {
    spdlog::info("GLProgram destroyed");
    destroy();
  }
  GLuint handle;
  std::unordered_map<std::string, GLuint> uniforms;
};