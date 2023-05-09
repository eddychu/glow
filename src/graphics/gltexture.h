#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

struct GLTextureConfig {
  GLenum target;
  int width;
  int height;
  int levels = 1;
  GLenum format;
  GLenum wrap_s;
  GLenum wrap_t;
  GLenum min_filter;
  GLenum mag_filter;
  glm::vec4 border_color;
};

class GLTexture {
public:
  explicit GLTexture(const GLTextureConfig &config);
  GLuint handle() const { return m_handle; }
  void destroy();

  void bind(int unit = 0) const;

private:
  GLTextureConfig m_config;
  GLuint m_handle;
};