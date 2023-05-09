#pragma once
#include <glad/glad.h>
#include <graphics/gltexture.h>
class GLFramebuffer {
public:
  GLFramebuffer(int width, int height);
  void attach(GLTexture &texture, GLenum attachment, int mip_level = 0);
  bool is_complete() const;
  void destroy();
  void bind() const;
  void unbind() const;
  GLuint handle() const { return m_handle; }

private:
  int m_width;
  int m_height;
  GLuint m_handle;
};