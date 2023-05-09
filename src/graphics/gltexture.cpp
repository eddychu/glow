#include <graphics/gltexture.h>
GLTexture::GLTexture(const GLTextureConfig &config) : m_config(config) {
  glCreateTextures(m_config.target, 1, &m_handle);
  glTextureStorage2D(m_handle, m_config.levels, m_config.format, m_config.width,
                     m_config.height);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, m_config.wrap_s);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, m_config.wrap_t);
  glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, m_config.min_filter);
  glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, m_config.mag_filter);
  glTextureParameterfv(m_handle, GL_TEXTURE_BORDER_COLOR,
                       &m_config.border_color[0]);
}

void GLTexture::destroy() { glDeleteTextures(1, &m_handle); }

void GLTexture::bind(int unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(m_config.target, m_handle);
}