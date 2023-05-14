#pragma once
#include <core/camera.h>
#include <resource/material.h>
#include <resource/resource.h>
#include <core/transform.h>
#include <memory>
#include <stdint.h>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
struct Node {
  int32_t mesh{-1};
  int32_t parent{-1};
  int32_t first_child{-1};
  uint32_t num_children{0};
  Transform transform;
};

/**
 * @brief A mesh is a collection of submeshes.
 *  this is equivalent to a primitive in glTF.
 */

struct SubMesh {
  int32_t material{-1};
  uint32_t geometry_id{0};
};

struct Mesh {
  std::vector<SubMesh> sub_meshes;
};

struct Scene : public Resource {
  Scene() : Resource(ResourceType::Scene) {}
  std::vector<Node> nodes;
  std::vector<Mesh> meshes;
  std::vector<Material> materials;
  Camera camera;
};

uint32_t load_scene(const std::string &filename, class ResourceCache *cache);