#pragma once
#include <core/bbox.h>
#include <scene/sid.h>
#include <core/transform.h>
struct Node : public SceneObject {
  int32_t mesh{-1};
  int32_t parent{-1};
  int32_t first_child{-1};
  uint32_t num_children{0};
  Transform transform;
  BBox bbox;
};