#include <graphics/glframebuffer.h>

GLFramebuffer::GLFramebuffer(int width, int height)
    : m_width(width), m_height(height) {
  glCreateFramebuffers(1, &m_handle);

  // TODO: currently disable draw buffers. need to handle this better
  //   GLenum draw_buffers[] = {GL_NONE};
  //   glNamedFramebufferDrawBuffers(m_handle, 1, draw_buffers);
}

void GLFramebuffer::attach(GLTexture &texture, GLenum attachment,
                           int mip_level) {
  glNamedFramebufferTexture(m_handle, attachment, texture.handle(), mip_level);
  GLenum draw_buffers[] = {GL_NONE};
  glNamedFramebufferDrawBuffers(m_handle, 1, draw_buffers);
}

void GLFramebuffer::destroy() { glDeleteFramebuffers(1, &m_handle); }

void GLFramebuffer::bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
  glViewport(0, 0, m_width, m_height);
}

void GLFramebuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

bool GLFramebuffer::is_complete() const {
  return glCheckNamedFramebufferStatus(m_handle, GL_FRAMEBUFFER) ==
         GL_FRAMEBUFFER_COMPLETE;
}