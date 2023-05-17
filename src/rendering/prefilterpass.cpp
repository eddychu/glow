// #include "geometry/geometry.h"
// #include "resource/geometry_buffer.h"
// #include <rendering/prefilterpass.h>
// #include <glm/ext.hpp>

// static mat4 projection =
//     glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

// static std::vector<mat4> views = {
//     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
//                 glm::vec3(0.0f, -1.0f, 0.0f)),
//     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
//                 glm::vec3(0.0f, -1.0f, 0.0f)),
//     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
//                 glm::vec3(0.0f, 0.0f, 1.0f)),
//     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
//                 glm::vec3(0.0f, 0.0f, -1.0f)),
//     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
//                 glm::vec3(0.0f, -1.0f, 0.0f)),
//     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
//                 glm::vec3(0.0f, -1.0f, 0.0f))};

// void PrefilterPass::init(GLProgram *program, TextureCube
// *environment_texture) {
//   this->program = program;
//   this->environment_texture = environment_texture;
//   glCreateFramebuffers(1, &capture_fbo);
//   glCreateRenderbuffers(1, &capture_rbo);

//   glNamedRenderbufferStorage(capture_rbo, GL_DEPTH_COMPONENT24, width,
//   height); glNamedFramebufferRenderbuffer(capture_fbo, GL_DEPTH_ATTACHMENT,
//                                  GL_RENDERBUFFER, capture_rbo);

//   glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &prefilter_texture);
//   glTextureStorage2D(prefilter_texture, mip_levels, GL_RGB16F, width,
//   height);

//   glTextureParameteri(prefilter_texture, GL_TEXTURE_MIN_FILTER,
//                       GL_LINEAR_MIPMAP_LINEAR);
//   glTextureParameteri(prefilter_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   glTextureParameteri(prefilter_texture, GL_TEXTURE_WRAP_S,
//   GL_CLAMP_TO_EDGE); glTextureParameteri(prefilter_texture,
//   GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   glTextureParameteri(prefilter_texture, GL_TEXTURE_WRAP_R,
//   GL_CLAMP_TO_EDGE); glGenerateTextureMipmap(prefilter_texture);

//   Geometry g = make_cube(2.0f);
//   this->geometry = new GeometryBuffer(g.vertices, g.indices);
// }

// void PrefilterPass::render() {
//   glUseProgram(program->handle);
//   glViewport(0, 0, width, height);
//   glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
//   glActiveTexture(GL_TEXTURE0);
//   glBindTexture(GL_TEXTURE_CUBE_MAP, environment_texture->handle());
//   program->set_uniform("projection", GLUniform{.value = projection});
//   for (int mip = 0; mip < mip_levels; mip++) {
//     unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5,
//     mip)); unsigned int mipHeight =
//         static_cast<unsigned int>(128 * std::pow(0.5, mip));
//     glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
//     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth,
//                           mipHeight);
//     glViewport(0, 0, mipWidth, mipHeight);
//     float roughness = (float)mip / (float)(mip_levels - 1);
//     program->set_uniform("roughness", GLUniform{.value = roughness});
//     for (int i = 0; i < 6; i++) {
//       program->set_uniform("view", GLUniform{.value = views[i]});
//       glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//                              GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
//                              prefilter_texture, mip);

//       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//       glBindVertexArray(this->geometry->handle());
//       glDrawElements(GL_TRIANGLES, this->geometry->count(),
//       GL_UNSIGNED_SHORT,
//                      0);
//     }
//   }
//   glBindFramebuffer(GL_FRAMEBUFFER, 0);
//   glUseProgram(0);
// }