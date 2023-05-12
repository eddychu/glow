// #include <geometry/geometry.h>
// #include <graphics/glbackground.h>
// #include <stdint.h>

// const char *VERTEX_SHADER_PATH = "assets/shaders/background.vert.glsl";
// const char *FRAGMENT_SHADER_PATH = "assets/shaders/background.frag.glsl";

// GLBackground::GLBackground()
//     : program(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH) {
//   create_geometry();
// }

// void GLBackground::draw() const {
//   program.use();
//   glBindVertexArray(vao);
//   glDrawArrays(GL_TRIANGLES, 0, count);
//   glBindVertexArray(0);
//   program.unuse();
// }

// void GLBackground::destroy() const {
//   glDeleteBuffers(1, &vbo);
//   glDeleteVertexArrays(1, &vao);
// }

// void GLBackground::create_geometry() {

//   // cube positions;
//   float positions[] = {
//       // positions
//       -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
//       1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

//       -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
//       -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

//       1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
//       1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

//       -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
//       1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

//       -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
//       1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

//       -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
//       1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

//   glCreateBuffers(1, &vbo);
//   glNamedBufferStorage(vbo, sizeof(positions), positions, 0);
//   glCreateVertexArrays(1, &vao);
//   glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 3);
//   glEnableVertexArrayAttrib(vao, 0);
//   glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
//   glVertexArrayAttribBinding(vao, 0, 0);
//   count = 36;
// }
