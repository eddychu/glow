#pragma once
#include <glad/glad.h>
class GLBuffer {
public:
  GLBuffer(const void *data, size_t size, GLenum usage = 0);

  void update(const void *data, size_t size, size_t offset = 0) const;

  void destroy() const;
  GLuint handle;
};