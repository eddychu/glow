#pragma once
#include <memory>
#include <resource/resource.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stdint.h>
#include <vector>
#include <glad/glad.h>
using namespace glm;

struct Vertex {
  vec3 position;
  vec3 normal;
  vec2 uv;
  vec3 tangent;
  vec3 bitangent;
};

class Geometry : public Resource {
public:
  static std::unique_ptr<Geometry> make_cube(float size = 1.0f) {
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

    std::vector<vec3> normals = {
        // Front
        vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f),
        // Right
        vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f),
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
        vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f)};

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
      Vertex v;
      v.position = positions[i];
      v.normal = normals[i];
      v.uv = uvs[i];
      vertices[i] = v;
    }

    return make_unique<Geometry>(vertices, el);
  }

  static std::unique_ptr<Geometry> quad(float size = 1.0f) {
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
      Vertex v;
      v.position = positions[i];
      v.normal = normals[i];
      v.uv = uvs[i];
      vertices[i] = v;
    }

    return make_unique<Geometry>(vertices, indices);
  }

  Geometry(const std::vector<Vertex> &vertices);
  Geometry(const std::vector<Vertex> &vertices,
           const std::vector<uint16_t> &indices);

  void destroy();

  ~Geometry();

  bool has_indices() const { return m_has_indices; }
  GLuint handle() const { return vao; }
  GLuint count() const { return m_count; }

private:
  void setup_vbo(const std::vector<Vertex> &vertices);
  GLuint vao{0};
  GLuint vbo{0};
  GLuint ebo{0};
  bool m_has_indices{false};
  GLuint m_count{0};
};