#pragma once

#include <core/camera.h>
#include <core/transform.h>
#include <core/bbox.h>
#include <memory>
#include <stdint.h>
#include <vector>
#include <string>
#include <light/light.h>
#include <scene/node.h>
#include <scene/mesh.h>
#include <scene/texture.h>
#include <scene/material.h>
#include <scene/geometry.h>

struct Scene  {
  std::vector<Node> nodes;
  std::vector<Mesh> meshes;
  std::vector<Material> materials;
  std::vector<Texture> textures;
  std::vector<Light> lights;
  std::unique_ptr<TextureCube> environment = nullptr;
};

Scene load_scene(const std::string &filename);