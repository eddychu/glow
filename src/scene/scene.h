#pragma once
#include "geometry/geometry.h"
#include <core/camera.h>
#include <scene/material.h>
#include <core/transform.h>
#include <memory>
#include <stdint.h>
#include <vector>
#include <string>
struct Sampler {
  enum class Filter {
    Nearest,
    Linear,
  };
  enum class Wrap { Repeat, MirroredRepeat, ClampToEdge, ClampToBorder };

  static Filter filter_from_int(int i) {
    switch (i) {
    case 9728:
      return Filter::Nearest;
    case 9729:
      return Filter::Linear;
    default:
      return Filter::Nearest;
    }
  }

  static Wrap wrap_from_int(int i) {
    switch (i) {
    case 10497:
      return Wrap::Repeat;
    case 33648:
      return Wrap::MirroredRepeat;
    case 33071:
      return Wrap::ClampToEdge;
    case 33069:
      return Wrap::ClampToBorder;
    default:
      return Wrap::Repeat;
    }
  }

  Filter min_filter{Filter::Nearest};
  Filter mag_filter{Filter::Nearest};
  Wrap wrap_s{Wrap::Repeat};
  Wrap wrap_t{Wrap::Repeat};
  Wrap wrap_r{Wrap::Repeat};
};

struct Texture {
  uint32_t width;
  uint32_t height;
  uint32_t component;
  std::vector<unsigned char> data;
  Sampler sampler;
};

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
  Geometry geometry;
};

struct Mesh {
  std::vector<SubMesh> sub_meshes;
};

struct Scene {
  std::vector<Node> nodes;
  std::vector<Mesh> meshes;
  std::vector<Material> materials;
  std::vector<Texture> textures;
};

Scene load_scene(const std::string &filename);