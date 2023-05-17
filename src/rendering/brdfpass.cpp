// #include "resource/shader.h"
// #include <rendering/brdfpass.h>

// void BRDFPass::init(GLProgram *program, GLuint capture_fbo,
//                     GLuint capture_rbo) {
//   this->program = program;
//   glCreateTextures(GL_TEXTURE_2D, 1, &brdf_texture);
//   glTextureStorage2D(brdf_texture, 1, GL_RG16F, width, height);
//   glTextureParameteri(brdf_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTextureParameteri(brdf_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   glTextureParameteri(brdf_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTextureParameteri(brdf_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//   float vertices[] = {
//       -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//       1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
//   };
//   glCreateVertexArrays(1, &vao);
//   glCreateBuffers(1, &vbo);
//   glNamedBufferStorage(vbo, sizeof(vertices), vertices, 0);
//   glVertexArrayVertexBuffer(vao, 0, vbo, 0, 5 * sizeof(float));
//   glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
//   glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 3 *
//   sizeof(float)); glEnableVertexArrayAttrib(vao, 0);
//   glEnableVertexArrayAttrib(vao, 1);
//   glVertexArrayAttribBinding(vao, 0, 0);
//   glVertexArrayAttribBinding(vao, 1, 0);

//   this->capture_fbo = capture_fbo;
//   this->capture_rbo = capture_rbo;
// }

// void BRDFPass::render() {
//   glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
//   glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
//   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width,
//   height); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//   GL_TEXTURE_2D,
//                          brdf_texture, 0);
//   glViewport(0, 0, width, height);
//   this->program->use();
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//   glBindVertexArray(vao);
//   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//   glBindVertexArray(0);
//   glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }