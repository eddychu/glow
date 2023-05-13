#pragma once
#include <resource/geometry.h>
#include <resource/material.h>
#include <resource/resource.h>
#include <resource/shader.h>
#include <resource/texture.h>
#include <cstdint>
#include <light/light.h>
#include <core/camera.h>
#include <stdint.h>
#include <unordered_map>
#include <vector>
struct PBRPass {
  Texture *albedo_texture{nullptr};
  Texture *normal_texture{nullptr};
  Texture *metallic_texture{nullptr};
  Texture *ao_texture{nullptr};
  Texture *emissive_texture{nullptr};

  Light *lights{nullptr};
  uint32_t light_count{0};

  GLProgram *program{nullptr};

  Camera *camera{nullptr};

  std::vector<Geometry *> geometries;

  uint32_t width{0};
  uint32_t height{0};

  uint32_t samples{0};

  void render();
};

std::vector<PBRPass> from_scene(uint32_t scene_id, ResourceCache *cache);