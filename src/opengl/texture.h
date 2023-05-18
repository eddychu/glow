#pragma once

#include <glad/glad.h>
#include <opengl/resource.h>
#include <scene/scene.h>
class GLTexture : public Resource {
public:
  GLTexture(const Texture &texture);

  void destroy() const;

  GLuint handle() const { return m_handle; }

  ~GLTexture() { destroy(); }

private:
  GLuint m_handle = 0;
};

class GLTextureCube {
public:
  GLTextureCube(const std::string &path, const std::string &file_extension);

  void destroy() const;

  GLuint handle() const { return m_handle; }

  ~GLTextureCube() { destroy(); }

private:
  GLuint m_handle;
};
