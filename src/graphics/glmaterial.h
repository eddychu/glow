// #pragma once
// #include <graphics/glshader.h>
// #include <glm/glm.hpp>
// class GLMaterial {
// public:
//   GLMaterial() = default;

//   GLMaterial(GLProgram *program) : m_program(program) {}

//   void set_shader(GLProgram *program) { m_program = program; }

// private:
//   GLProgram *m_program{nullptr};
// };

// class GLPhongMaterial : public GLMaterial {
// public:
//   GLPhongMaterial() = default;
//   void populate_uniforms() const;

// private:
//   GLProgram *m_program{nullptr};
//   glm::vec3 m_ambient{0.1f, 0.1f, 0.1f};
//   glm::vec3 m_diffuse{0.5f, 0.5f, 0.5f};
//   glm::vec3 m_specular{0.5f, 0.5f, 0.5f};
// };