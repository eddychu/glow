#include <loaders/texture_loader.h>
#include <stb_image.h>
#include <stdint.h>
#include <spdlog/spdlog.h>
HDRTexture load_hdr_texture(const std::string &file, bool flip) {
  HDRTexture texture;
  stbi_set_flip_vertically_on_load(flip);
  int width, height, component;
  float *data = stbi_loadf(file.c_str(), &width, &height, &component, 4);
  if (!data) {
    spdlog::error("Failed to load texture: {}", file);
    throw std::runtime_error("Failed to load texture: " + file);
  }
  texture.data = std::vector<float>(texture.width * texture.height * 4);
  texture.width = width;
  texture.height = height;
  texture.component = 4;
  memcpy(texture.data.data(), data,
         texture.width * texture.height * texture.component * sizeof(float));
  stbi_image_free(data);
  stbi_set_flip_vertically_on_load(false);
  return texture;
}

Texture load_texture(const std::string &file, bool flip) {
  Texture texture;
  stbi_set_flip_vertically_on_load(flip);
  int width, height, component;
  uint8_t *data = stbi_load(file.c_str(), &width, &height, &component, 4);
  if (!data) {
    spdlog::error("Failed to load texture: {}", file);
    throw std::runtime_error("Failed to load texture: " + file);
  }
  texture.data = std::vector<uint8_t>(texture.width * texture.height * 4);
  texture.width = width;
  texture.height = height;
  texture.component = 4;
  memcpy(texture.data.data(), data,
         texture.width * texture.height * texture.component * sizeof(uint8_t));
  stbi_image_free(data);
  stbi_set_flip_vertically_on_load(false);
  return texture;
}