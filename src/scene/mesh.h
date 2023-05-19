#pragma once

#include <scene/sid.h>
#include <vector>
#include <core/bbox.h>
#include <scene/geometry.h>
struct SubMesh : public SceneObject {
  int32_t material{-1};
  Geometry geometry;
  BBox bbox;
};

struct Mesh : public SceneObject {
  std::vector<SubMesh> sub_meshes;
  BBox bbox;
};