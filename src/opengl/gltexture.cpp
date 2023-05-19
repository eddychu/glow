#include "scene/texture.h"
#include <cstdlib>
#include <opengl/gltexture.h>
#include <variant>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <spdlog/spdlog.h>

static GLenum get_format_from_component(int component) {
  switch (component) {
  case 1:
    return GL_RED;
  case 2:
    return GL_RG;
  case 3:
    return GL_RGB;
  case 4:
    return GL_RGBA;
  default:
    spdlog::error("Invalid texture component {}", component);
    throw std::runtime_error("Invalid texture component");
  }
}

static GLenum get_internal_format_from_component(int component) {
  switch (component) {
  case 1:
    return GL_R8;
  case 2:
    return GL_RG8;
  case 3:
    return GL_RGB8;
  case 4:
    return GL_RGBA8;
  default:
    spdlog::error("Invalid texture component: {}", component);
    throw std::runtime_error("Invalid texture component");
  }
}

static GLenum get_type_from_texture(const PixelDataArray &data) {
  if(std::holds_alternative<LDR>(data)) {
    return GL_UNSIGNED_BYTE;
  } else {
    return GL_FLOAT;
  }
}

static GLenum get_filter(Sampler::Filter filter) {
  switch (filter) {
  case Sampler::Filter::Nearest:
    return GL_NEAREST;
  case Sampler::Filter::Linear:
    return GL_LINEAR;
  default:
    spdlog::error("Invalid filter ");
    throw std::runtime_error("Invalid filter");
  }
}


static GLenum get_wrap(Sampler::Wrap wrap) {
  switch (wrap) {
  case Sampler::Wrap::Repeat:
    return GL_REPEAT;
  case Sampler::Wrap::MirroredRepeat:
    return GL_MIRRORED_REPEAT;
  case Sampler::Wrap::ClampToEdge:
    return GL_CLAMP_TO_EDGE;
  case Sampler::Wrap::ClampToBorder:
    return GL_CLAMP_TO_BORDER;
  default:
    spdlog::error("Invalid wrap ");
    throw std::runtime_error("Invalid wrap");
  }
}


GLTexture::GLTexture(const Texture &texture) {
  id = texture.get_id();
  GLenum target = GL_TEXTURE_2D;
  GLenum internal_format = get_internal_format_from_component(texture.component);
  GLenum format = get_format_from_component(texture.component);
  GLenum type = get_type_from_texture(texture.data);
  void* data = nullptr;
  if(std::holds_alternative<LDR>(texture.data)) {
    data = (void *)std::get<LDR>(texture.data).data();
  } else {
    data = (void *) std::get<HDR>(texture.data).data();
  }

  GLenum min_filter = get_filter(texture.sampler.min_filter);
  GLenum mag_filter = get_filter(texture.sampler.mag_filter);
  GLenum wrap_s = get_wrap(texture.sampler.wrap_s);
  GLenum wrap_t = get_wrap(texture.sampler.wrap_t);

  glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
  glTextureStorage2D(m_handle, 1, internal_format, texture.width,
                     texture.height);
  glTextureSubImage2D(m_handle, 0, 0, 0, texture.width, texture.height, format,
                      type, data);
  glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, min_filter);
  glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, mag_filter);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, wrap_s);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, wrap_t);
}


void GLTexture::destroy() const {
  spdlog::info("texture destroyed");
  glDeleteTextures(1, &m_handle);
}

GLTextureCube::GLTextureCube(const TextureCube& texture) {
  GLenum target = GL_TEXTURE_CUBE_MAP;
  GLenum internal_format = get_internal_format_from_component(texture.component);
  GLenum format = get_format_from_component(texture.component);
  GLenum type = get_type_from_texture(texture.data[0]);

  glCreateTextures(target, 1, &m_handle);
  glTextureStorage2D(m_handle, 1, internal_format, texture.width,
                     texture.height);
  for (int i = 0; i < 6; i++) {
    auto& face = texture.data[i];
    void* data = nullptr;
    if(std::holds_alternative<LDR>(face)) {
      data = (void *)std::get<LDR>(face).data();
    } else {
      data = (void *) std::get<HDR>(face).data();
    }
    glTextureSubImage3D(m_handle, 0, 0, 0, i, texture.width,
                        texture.height, 1, format, type,
                        data);
  }

  GLenum min_filter = get_filter(texture.sampler.min_filter);
  GLenum mag_filter = get_filter(texture.sampler.mag_filter);
  GLenum wrap_s = get_wrap(texture.sampler.wrap_s);
  GLenum wrap_t = get_wrap(texture.sampler.wrap_t);
  GLenum wrap_r = get_wrap(texture.sampler.wrap_r);

  glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, min_filter);
  glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, mag_filter);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, wrap_s);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, wrap_t);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_R, wrap_r);
}

void GLTextureCube::destroy() const {
  spdlog::info("texture destroyed");
  glDeleteTextures(1, &m_handle);
}
