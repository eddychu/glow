#pragma once
#include <core/bbox.h>
#include <scene/resource.h>
#include <glm/glm.hpp>
#include <stdint.h>
using namespace glm;
#include <vector>

struct Vertex {
  vec3 position;
  vec3 normal;
  vec2 uv;
  vec3 tangent;
  vec3 bitangent;
};

enum class GeometryMode {
  Triangles,
  Lines,
  Points,
};

struct Geometry : public SceneResource {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  GeometryMode mode = GeometryMode::Triangles;
};

Geometry make_quad(float size);

Geometry make_cube(float size);

Geometry make_grid(int size, float step);

Geometry make_axis(float size);

Geometry make_bbox(const BBox &bbox);