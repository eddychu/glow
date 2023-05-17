#pragma once
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

struct Geometry {
  std::vector<Vertex> vertices;
  std::vector<uint16_t> indices;
  GeometryMode mode = GeometryMode::Triangles;
};

Geometry make_quad(float size);

Geometry make_cube(float size);

Geometry make_grid(int size, float step);