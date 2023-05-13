#pragma once

#include <glad/glad.h>
#include <resource/resource.h>

struct TextureConfig {
  GLenum internal_format = GL_RGBA8;
  GLenum format = GL_RGBA;
  GLsizei levels = 1;
  ResourceType type = ResourceType::Texture2d;
  GLenum wrap_s = GL_REPEAT;
  GLenum wrap_t = GL_REPEAT;
  GLenum min_filter = GL_LINEAR;
  GLenum mag_filter = GL_LINEAR;
};

class Texture : public Resource {
public:
  Texture(const std::string &path, const TextureConfig &config);

  void destroy() const;

  GLuint handle() const { return m_handle; }

  ~Texture() { destroy(); }

private:
  GLuint m_handle;
};

class TextureCube : public Resource {
public:
  TextureCube(const std::string &path, const std::string &file_extension,
              const TextureConfig &config);

  void destroy() const;

  GLuint handle() const { return m_handle; }

  ~TextureCube() { destroy(); }

private:
  GLuint m_handle;
};
