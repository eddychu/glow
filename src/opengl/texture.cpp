#include <cstdlib>
#include <opengl/resource.h>
#include <opengl/texture.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <spdlog/spdlog.h>
GLTexture::GLTexture(const Texture &texture) {
  set_id(texture.id());
  GLenum target = GL_TEXTURE_2D;
  GLenum internal_format = GL_RGBA8;
  GLenum format = GL_RGBA;

  switch (texture.component) {
  case 1: {
    internal_format = GL_R8;
    format = GL_RED;
    break;
  }
  case 2: {
    internal_format = GL_RG8;
    format = GL_RG;
    break;
  }
  case 3: {
    internal_format = GL_RGB8;
    format = GL_RGB;
    break;
  }
  case 4: {
    internal_format = GL_RGBA8;
    format = GL_RGBA;
    break;
  }
  default: {
    spdlog::error("Invalid texture component: {}", texture.component);
    throw std::runtime_error("Invalid texture component");
  }
  }

  glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
  glTextureStorage2D(m_handle, 1, internal_format, texture.width,
                     texture.height);
  glTextureSubImage2D(m_handle, 0, 0, 0, texture.width, texture.height, GL_RGBA,
                      GL_UNSIGNED_BYTE, texture.data.data());
  glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void GLTexture::destroy() const {
  spdlog::info("texture destroyed");
  glDeleteTextures(1, &m_handle);
}

GLTextureCube::GLTextureCube(const std::string &path,
                             const std::string &file_extension) {
  GLenum target = GL_TEXTURE_CUBE_MAP;
  spdlog::info("Loading texture: {}", path);
  int width, height, nrChannels;
  const char *names[] = {"posx", "negx", "posy", "negy", "posz", "negz"};
  if (file_extension == "hdr") {
    float *data;
    std::string full_path = path + "/" + names[0] + "." + file_extension;
    data = stbi_loadf(full_path.c_str(), &width, &height, &nrChannels, 4);
    if (!data) {
      spdlog::error("Failed to load texture: {}", full_path);
      throw std::runtime_error("Failed to load texture");
    }
    glCreateTextures(target, 1, &m_handle);
    glTextureStorage2D(m_handle, 1, GL_RGBA32F, width, height);
    glTextureSubImage3D(m_handle, 0, 0, 0, 0, width, height, 1, GL_RGBA,
                        GL_FLOAT, data);
    stbi_image_free(data);
    for (int i = 1; i < 6; i++) {
      std::string full_path = path + "/" + names[i] + "." + file_extension;
      data = stbi_loadf(full_path.c_str(), &width, &height, &nrChannels, 4);
      if (!data) {
        spdlog::error("Failed to load texture: {}", full_path);
        throw std::runtime_error("Failed to load texture");
      }
      glTextureSubImage3D(m_handle, 0, 0, 0, i, width, height, 1, GL_RGBA,
                          GL_FLOAT, data);

      stbi_image_free(data);
    }
  } else {
    unsigned char *data;
    std::string full_path = path + "/" + names[0] + "." + file_extension;
    data = stbi_load(full_path.c_str(), &width, &height, &nrChannels, 4);
    if (!data) {
      spdlog::error("Failed to load texture: {}", full_path);
      throw std::runtime_error("Failed to load texture");
    }
    glCreateTextures(target, 1, &m_handle);
    glTextureStorage2D(m_handle, 1, GL_RGBA8, width, height);
    glTextureSubImage3D(m_handle, 0, 0, 0, 0, width, height, 1, GL_RGBA,
                        GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    for (int i = 1; i < 6; i++) {
      std::string full_path = path + "/" + names[i] + "." + file_extension;
      data = stbi_load(full_path.c_str(), &width, &height, &nrChannels, 4);
      if (!data) {
        spdlog::error("Failed to load texture: {}", full_path);
        throw std::runtime_error("Failed to load texture");
      }
      glTextureSubImage3D(m_handle, 0, 0, 0, i, width, height, 1, GL_RGBA,
                          GL_UNSIGNED_BYTE, data);

      stbi_image_free(data);
    }
  }

  glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void GLTextureCube::destroy() const {
  spdlog::info("texture destroyed");
  glDeleteTextures(1, &m_handle);
}
