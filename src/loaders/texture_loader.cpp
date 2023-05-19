#include "scene/texture.h"
#include <loaders/texture_loader.h>
#include <memory>
#include <stb_image.h>
#include <stdint.h>
#include <spdlog/spdlog.h>
#include <vcruntime_string.h>

std::unique_ptr<Texture> load_texture(const std::string &file, bool flip) {
  auto texture = std::make_unique<Texture>();
  stbi_set_flip_vertically_on_load(flip);
  int width, height, _ignore;
  
  if (stbi_is_hdr(file.c_str())) {
    float* pixel_data = stbi_loadf(file.c_str(), &width, &height, &_ignore, 4);
    if (!pixel_data) {
      spdlog::error("Failed to load texture: {}", file);
      throw std::runtime_error("Failed to load texture: " + file);
    }
    std::vector<float> data(width * height * 4);
    memcpy(data.data(), pixel_data,
           width * height * 4 * sizeof(float));
    
    texture->data = data;
    stbi_image_free(pixel_data);
  } else {
    uint8_t* pixel_data = stbi_load(file.c_str(), &width, &height, &_ignore, 4);
    if (!pixel_data) {
      spdlog::error("Failed to load texture: {}", file);
      throw std::runtime_error("Failed to load texture: " + file);
    }
    std::vector<uint8_t> data(width * height * 4);
    memcpy(data.data(), pixel_data,
           width * height * 4 * sizeof(uint8_t));
    texture->data = data;
    stbi_image_free(pixel_data);
  }
  texture->width = width;
  texture->height = height;
  texture->component = 4;

  
  stbi_set_flip_vertically_on_load(false);
  return texture;
}


std::unique_ptr<TextureCube> load_texture_cube(const std::vector<std::string> files, bool flip) {
  auto texture = std::make_unique<TextureCube>();
  stbi_set_flip_vertically_on_load(flip);
  int width, height, _ignore;
  texture->data.resize(6);
  for (int i = 0; i < 6; i++) {
    if (stbi_is_hdr(files[i].c_str())) {
      float* pixel_data = stbi_loadf(files[i].c_str(), &width, &height, &_ignore, 4);
      if (!pixel_data) {
        spdlog::error("Failed to load texture: {}", files[i]);
        throw std::runtime_error("Failed to load texture: " + files[i]);
      }
      std::vector<float> data(width * height * 4);
      memcpy(data.data(), pixel_data,
             width * height * 4 * sizeof(float));
      texture->data[i] = data;
      stbi_image_free(pixel_data);
    } else {
      uint8_t* pixel_data = stbi_load(files[i].c_str(), &width, &height, &_ignore, 4);
      if (!pixel_data) {
        spdlog::error("Failed to load texture: {}", files[i]);
        throw std::runtime_error("Failed to load texture: " + files[i]);
      }
      std::vector<uint8_t> data(width * height * 4);
      memcpy(data.data(), pixel_data,
             width * height * 4 * sizeof(uint8_t));
      texture->data[i] = data;
      stbi_image_free(pixel_data);
    }
  }
  texture->width = width;
  texture->height = height;
  texture->component = 4;
  stbi_set_flip_vertically_on_load(false);
  return texture;
}