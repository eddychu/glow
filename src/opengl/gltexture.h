#pragma once
#include <opengl/globject.h>
#include <scene/texture.h>
class GLTexture : public GLObject{
public:
  GLTexture(const Texture &texture);
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

class GLTextureCube : public GLObject {
public:
  GLTextureCube(const TextureCube& texture);

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
