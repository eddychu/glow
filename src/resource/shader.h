#pragma once
#include <resource/resource.h>
#include <glad/glad.h>
#include <variant>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <spdlog/spdlog.h>
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

struct GLUniform {
  std::variant<mat4, mat3, vec4, vec3, vec2, float, int> value;
};

class GLProgram : public Resource {
public:
  GLProgram(const char *vs_path, const char *fs_path);
  void populate_uniforms();
  void set_uniform(const char *name, const GLUniform &uniform) const;

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