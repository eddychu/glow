// #include <cstddef>
// #include <graphics/glgeometry.h>

// GLGeometry::GLGeometry(const std::vector<Vertex> &vertices) {
//   hasIndices = false;
//   glCreateBuffers(1, &vbo);
//   glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex),
//   vertices.data(),
//                        0);
//   glCreateVertexArrays(1, &vao);
//   glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
//   glEnableVertexArrayAttrib(vao, 0);
//   glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE,
//                             offsetof(Vertex, position));
//   glVertexArrayAttribBinding(vao, 0, 0);
//   glEnableVertexArrayAttrib(vao, 1);
//   glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE,
//                             offsetof(Vertex, normal));
//   glVertexArrayAttribBinding(vao, 1, 0);
//   glEnableVertexArrayAttrib(vao, 2);
//   glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE,
//                             offsetof(Vertex, uv));
//   glVertexArrayAttribBinding(vao, 2, 0);
//   count = vertices.size();
// }

// GLGeometry::GLGeometry(const std::vector<Vertex> &vertices,
//                        const std::vector<uint16_t> &indices) {
//   hasIndices = true;
//   glCreateBuffers(1, &vbo);
//   glNamedBufferStorage(vbo, vertices.size() * sizeof(Vertex),
//   vertices.data(),
//                        0);
//   glCreateBuffers(1, &ibo);
//   glNamedBufferStorage(ibo, indices.size() * sizeof(uint16_t),
//   indices.data(),
//                        0);
//   glCreateVertexArrays(1, &vao);
//   glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
//   glEnableVertexArrayAttrib(vao, 0);
//   glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE,
//                             offsetof(Vertex, position));
//   glVertexArrayAttribBinding(vao, 0, 0);
//   glEnableVertexArrayAttrib(vao, 1);
//   glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE,
//                             offsetof(Vertex, normal));
//   glVertexArrayAttribBinding(vao, 1, 0);
//   glEnableVertexArrayAttrib(vao, 2);
//   glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE,
//                             offsetof(Vertex, uv));
//   glVertexArrayAttribBinding(vao, 2, 0);
//   glVertexArrayElementBuffer(vao, ibo);
//   count = indices.size();
// }

// void GLGeometry::destroy() const {

//   glDeleteBuffers(1, &vbo);
//   if (hasIndices) {
//     glDeleteBuffers(1, &ibo);
//   }
//   glDeleteVertexArrays(1, &vao);
// }

// void GLGeometry::draw(const GLProgram &program) const {
//   program.use();
//   glBindVertexArray(vao);
//   if (hasIndices) {
//     glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
//   } else {
//     glDrawArrays(GL_TRIANGLES, 0, count);
//   }
// }
