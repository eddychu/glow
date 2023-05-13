#include "resource/resource.h"
#include <resource/texture.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <spdlog/spdlog.h>
Texture::Texture(const std::string &path, const TextureConfig &config)
    : Resource(config.type) {
  GLenum target = GL_TEXTURE_2D;
  // TODO: Add support for other texture types.
  //   if (config.type == ResourceType::TextureCube) {
  //     target = GL_TEXTURE_CUBE_MAP;
  //   }
  spdlog::info("Loading texture: {}", path);
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
  if (!data) {
    spdlog::error("Failed to load texture: {}", path);
    throw std::runtime_error("Failed to load texture");
  }
  glCreateTextures(target, 1, &m_handle);
  glTextureStorage2D(m_handle, config.levels, GL_RGBA8, width, height);
  glTextureSubImage2D(m_handle, 0, 0, 0, width, height, GL_RGBA,
                      GL_UNSIGNED_BYTE, data);
  glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_R, GL_REPEAT);
  stbi_image_free(data);
}

void Texture::destroy() const {
  spdlog::info("texture destroyed");
  glDeleteTextures(1, &m_handle);
}

TextureCube::TextureCube(const std::string &path,
                         const std::string &file_extension,
                         const TextureConfig &config)
    : Resource(config.type) {
  GLenum target = GL_TEXTURE_CUBE_MAP;
  spdlog::info("Loading texture: {}", path);
  int width, height, nrChannels;
  const char *names[] = {"posx", "negx", "posy", "negy", "posz", "negz"};
  unsigned char *data;
  std::string full_path = path + "/" + names[0] + "." + file_extension;
  data = stbi_load(full_path.c_str(), &width, &height, &nrChannels, 4);
  if (!data) {
    spdlog::error("Failed to load texture: {}", full_path);
    throw std::runtime_error("Failed to load texture");
  }
  glCreateTextures(target, 1, &m_handle);
  glTextureStorage2D(m_handle, config.levels, GL_RGBA8, width, height);
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
  glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(m_handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureCube::destroy() const {
  spdlog::info("texture destroyed");
  glDeleteTextures(1, &m_handle);
}
