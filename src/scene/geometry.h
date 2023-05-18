#pragma once
#include "glm/fwd.hpp"
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
  // bool has_tangents = false;
  // void calculuate_tangents() {
  //   if (mode != GeometryMode::Triangles || indices.size() == 0 ||
  //       has_tangents) {
  //     return;
  //   }
  //   for (uint32 i = 0; i < indices.size(); i++) {
  //     calculate_tangent_for_vertex(indices[i]);
  //   }
  // }

  // void calculate_tangent_for_vertex(uint32_t index) {
  //   auto &vertex = vertices[index];

  //   vec3 tangent = vec3(0.0f);
  //   vec3 bitangent = vec3(0.0f);
  //   uint32_t triangles_count = 0;
  //   for (uint32_t i = 0; i < indices.size(); i += 3) {
  //     uint32_t i0 = indices[i];
  //     uint32_t i1 = indices[i + 1];
  //     uint32_t i2 = indices[i + 2];

  //     if (i0 == index || i1 == index || i2 == index) {
  //       auto &v0 = vertices[i0];
  //       auto &v1 = vertices[i1];
  //       auto &v2 = vertices[i2];
  //       vec3 &pos0 = v0.position;
  //       vec3 &pos1 = v1.position;
  //       vec3 &pos2 = v2.position;

  //       vec2 &uv0 = v0.uv;
  //       vec2 &uv1 = v1.uv;
  //       vec2 &uv2 = v2.uv;

  //       vec3 delta_pos1 = pos1 - pos0;
  //       vec3 delta_pos2 = pos2 - pos0;

  //       vec2 delta_uv1 = uv1 - uv0;
  //       vec2 delta_uv2 = uv2 - uv0;

  //       float r = 1.0 / (delta_uv1.x * delta_uv2.y - delta_uv1.y *
  //       delta_uv2.x); tangent += (delta_pos1 * delta_uv2.y - delta_pos2 *
  //       delta_uv1.y) * r; bitangent += (delta_pos2 * delta_uv1.x - delta_pos1
  //       * delta_uv2.x) * r; triangles_count++;
  //     }
  //   }
  //   if (triangles_count > 0) {
  //     tangent = normalize(tangent / (float)triangles_count);
  //     bitangent = normalize(bitangent / (float)triangles_count);
  //   }

  //   vertex.tangent = tangent;
  //   vertex.bitangent = bitangent;
  // }
};

Geometry make_quad(float size);

Geometry make_cube(float size);

Geometry make_grid(int size, float step);