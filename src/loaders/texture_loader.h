#pragma once
#include <scene/scene.h>

HDRTexture load_hdr_texture(const std::string &file, bool flip = false);

Texture load_texture(const std::string &file, bool flip = false);