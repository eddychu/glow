#pragma once
#include <scene/texture.h>
#include <string>
#include <memory>
std::unique_ptr<Texture> load_texture(const std::string &file, bool flip = false);

std::unique_ptr<TextureCube> load_texture_cube(const std::vector<std::string> files, bool flip = false);