#include <graphics/glbuffer.h>

GLBuffer::GLBuffer(const void *data, size_t size, GLenum usage) {
  glCreateBuffers(1, &handle);
  glNamedBufferStorage(handle, size, data, usage);
}

void GLBuffer::update(const void *data, size_t size, size_t offset) const {
  glNamedBufferSubData(handle, offset, size, data);
}

void GLBuffer::destroy() const { glDeleteBuffers(1, &handle); }