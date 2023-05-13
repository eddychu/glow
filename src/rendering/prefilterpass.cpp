#include <rendering/prefilterpass.h>
#include <glm/ext.hpp>
void PrefilterPass::init(GLProgram *program, TextureCube *environment_texture) {
  this->program = program;
  this->environment_texture = environment_texture;
  glCreateFramebuffers(1, &capture_fbo);
  glCreateRenderbuffers(1, &capture_rbo);

  glNamedRenderbufferStorage(capture_rbo, GL_DEPTH_COMPONENT24, width, height);
  glNamedFramebufferRenderbuffer(capture_fbo, GL_DEPTH_ATTACHMENT,
                                 GL_RENDERBUFFER, capture_rbo);

  glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &prefilter_texture);
  glTextureStorage2D(prefilter_texture, mip_levels, GL_RGB16F, width, height);
  //   for (int i = 0; i < 6; i++) {
  //     // glTextureImage2D(prefilter_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X +
  //     i,
  //     // 0,
  //     //  GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
  //     glTextureSubImage3D(prefilter_texture, 0, 0, 0, i, width, height, 1,
  //     GL_RGB,
  //                         GL_FLOAT, nullptr);
  //   }
  glTextureParameteri(prefilter_texture, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
  glTextureParameteri(prefilter_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(prefilter_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(prefilter_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(prefilter_texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glGenerateTextureMipmap(prefilter_texture);

  std::vector<vec3> positions = {
      // Front
      vec3(-1.0f, -1.0f, 1.0f), vec3(1.0f, -1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f),
      vec3(-1.0f, 1.0f, 1.0f),
      // Right
      vec3(1.0f, -1.0f, 1.0f), vec3(1.0f, -1.0f, -1.0f),
      vec3(1.0f, 1.0f, -1.0f), vec3(1.0f, 1.0f, 1.0f),
      // Back
      vec3(-1.0f, -1.0f, -1.0f), vec3(-1.0f, 1.0f, -1.0f),
      vec3(1.0f, 1.0f, -1.0f), vec3(1.0f, -1.0f, -1.0f),
      // Left
      vec3(-1.0f, -1.0f, 1.0f), vec3(-1.0f, 1.0f, 1.0f),
      vec3(-1.0f, 1.0f, -1.0f), vec3(-1.0f, -1.0f, -1.0f),
      // Bottom
      vec3(-1.0f, -1.0f, 1.0f), vec3(-1.0f, -1.0f, -1.0f),
      vec3(1.0f, -1.0f, -1.0f), vec3(1.0f, -1.0f, 1.0f),
      // Top
      vec3(-1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, -1.0f),
      vec3(-1.0f, 1.0f, -1.0f)};
  std::vector<uint16_t> el = {0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,
                              8,  9,  10, 8,  10, 11, 12, 13, 14, 12, 14, 15,
                              16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};

  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &ebo);
  glCreateBuffers(1, &vbo);
  glNamedBufferStorage(vbo, positions.size() * sizeof(vec3), positions.data(),
                       0);
  glNamedBufferStorage(ebo, el.size() * sizeof(uint16_t), el.data(), 0);
  glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(vec3));
  glVertexArrayElementBuffer(vao, ebo);
  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
  count = el.size();
}

void PrefilterPass::render() {
  glUseProgram(program->handle);
  glViewport(0, 0, width, height);
  glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, environment_texture->handle());
  program->set_uniform("projection", GLUniform{.value = projection});
  for (int mip = 0; mip < mip_levels; mip++) {
    unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
    unsigned int mipHeight =
        static_cast<unsigned int>(128 * std::pow(0.5, mip));
    glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth,
                          mipHeight);
    glViewport(0, 0, mipWidth, mipHeight);
    float roughness = (float)mip / (float)(mip_levels - 1);
    program->set_uniform("roughness", GLUniform{.value = roughness});
    for (int i = 0; i < 6; i++) {
      program->set_uniform("view", GLUniform{.value = views[i]});
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             prefilter_texture, mip);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glUseProgram(0);
}