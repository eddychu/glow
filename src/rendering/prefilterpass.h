// #pragma once

// #include "resource/geometry_buffer.h"
// #include <resource/texture.h>
// #include <resource/shader.h>
// #include <stdint.h>
// #include <glm/glm.hpp>
// #include <glm/ext.hpp>
// struct PrefilterPass {

//   // TextureCube *texture{nullptr};
//   uint32_t mip_levels{5};

//   GLProgram *program{nullptr};
//   TextureCube *environment_texture{nullptr};

//   int width{128};
//   int height{128};

//   GLuint capture_fbo{0};
//   GLuint capture_rbo{0};

//   GLuint prefilter_texture{0};

//   GeometryBuffer *geometry{nullptr};

//   void init(GLProgram *program, TextureCube *environment_texture);

//   void render();
// };