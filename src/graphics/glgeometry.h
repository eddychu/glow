// #pragma once
// #include <glad/glad.h>
// #include <glm/glm.hpp>
// #include <geometry/geometry.h>
// #include <graphics/glshader.h>
// #include <stdint.h>
// #include <vector>

// using namespace glm;

// class GLGeometry {
// public:
//   explicit GLGeometry(const std::vector<Vertex> &vertices);
//   GLGeometry(const std::vector<Vertex> &vertices,
//              const std::vector<uint16_t> &indices);

//   void destroy() const;
//   void draw(const GLProgram &program) const;
//   GLuint vao;
//   GLuint vbo;
//   GLuint ibo = 0;
//   GLuint count;
//   bool hasIndices;
// };