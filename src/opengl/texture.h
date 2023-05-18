#pragma once

#include <glad/glad.h>
#include <opengl/resource.h>
#include <scene/scene.h>
#include <stdint.h>
class GLTexture : public Resource {
public:
  GLTexture(const Texture &texture);

  GLTexture(const HDRTexture &texture);

  void destroy() const;

  void bind(uint32_t slot = 0) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_handle);
  }

  GLuint handle() const { return m_handle; }

  ~GLTexture() { destroy(); }

private:
  GLuint m_handle = 0;
};

class GLTextureCube {
public:
  GLTextureCube(const std::vector<Texture> &textures);

  void destroy() const;

  void bind(uint32_t slot = 0) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
  }

  GLuint handle() const { return m_handle; }

  ~GLTextureCube() { destroy(); }

private:
  GLuint m_handle;
};
