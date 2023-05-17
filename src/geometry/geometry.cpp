#include <geometry/geometry.h>
#include <vector>

// TODO: currently they are missing tangent and binormal vectors.

Geometry make_quad(float size) {
  std::vector<vec3> positions = {
      {-size, 0.0f, -size},
      {-size, 0.0f, size},
      {size, 0.0f, size},
      {size, 0.0f, -size},
  };
  std::vector<vec3> normals = {
      {0.0f, 1.0f, 0.0f},
      {0.0f, 1.0f, 0.0f},
      {0.0f, 1.0f, 0.0f},
      {0.0f, 1.0f, 0.0f},
  };
  std::vector<vec2> uvs = {
      {0.0f, 0.0f},
      {0.0f, 1.0f},
      {1.0f, 1.0f},
      {1.0f, 0.0f},
  };

  std::vector<uint16_t> indices = {0, 1, 2, 0, 2, 3};

  std::vector<Vertex> vertices(positions.size());
  for (int i = 0; i < positions.size(); i++) {
    vertices[i].position = positions[i];
    vertices[i].normal = normals[i];
    vertices[i].uv = uvs[i];
  }

  return {
      vertices,
      indices,
      GeometryMode::Triangles,
  };
}

Geometry make_cube(float size) {
  float side2 = size / 2.0f;

  std::vector<vec3> positions = {
      // Front
      vec3(-side2, -side2, side2), vec3(side2, -side2, side2),
      vec3(side2, side2, side2), vec3(-side2, side2, side2),
      // Right
      vec3(side2, -side2, side2), vec3(side2, -side2, -side2),
      vec3(side2, side2, -side2), vec3(side2, side2, side2),
      // Back
      vec3(-side2, -side2, -side2), vec3(-side2, side2, -side2),
      vec3(side2, side2, -side2), vec3(side2, -side2, -side2),
      // Left
      vec3(-side2, -side2, side2), vec3(-side2, side2, side2),
      vec3(-side2, side2, -side2), vec3(-side2, -side2, -side2),
      // Bottom
      vec3(-side2, -side2, side2), vec3(-side2, -side2, -side2),
      vec3(side2, -side2, -side2), vec3(side2, -side2, side2),
      // Top
      vec3(-side2, side2, side2), vec3(side2, side2, side2),
      vec3(side2, side2, -side2), vec3(-side2, side2, -side2)};

  std::vector<vec3> normals = {// Front
                               vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
                               vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
                               // Right
                               vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),
                               vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),
                               // Back
                               vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, -1.0f),
                               vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, -1.0f),
                               // Left
                               vec3(-1.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f),
                               vec3(-1.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f),
                               // Bottom
                               vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f),
                               vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f),
                               // Top
                               vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),
                               vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)};

  std::vector<vec2> uvs = {
      // Front
      vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f),
      // Right
      vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f),
      // Back
      vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f),
      // Left
      vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f),
      // Bottom
      vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f),
      // Top
      vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f)};

  std::vector<uint16_t> el = {0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,
                              8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,
                              16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};

  std::vector<Vertex> vertices(positions.size());
  for (int i = 0; i < positions.size(); i++) {
    vertices[i].position = positions[i];
    vertices[i].normal = normals[i];
    vertices[i].uv = uvs[i];
  }

  return {vertices, el, GeometryMode::Triangles};
}

Geometry make_grid(int divide_x, int divide_z) {

  std::vector<Vertex> vertices;

  float x_step = 1.0f / divide_x;
  float z_step = 1.0f / divide_z;
  float x_start = -0.5f;
  float z_start = -0.5f;

  for (int i = 0; i <= divide_x; i++) {
    for (int j = 0; j <= divide_z; j++) {
      Vertex v;
      v.position = vec3(x_start + i * x_step, 0.0f, z_start + j * z_step);
      v.normal = vec3(0.0f, 1.0f, 0.0f);
      v.uv = vec2(i * x_step, j * z_step);
      vertices.push_back(v);
    }
  }

  return {vertices, {}, GeometryMode::Points};
}