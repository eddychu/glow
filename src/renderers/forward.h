#pragma once
#include "scene/geometry.h"
#include <memory>
#include <scene/material.h>
#include <opengl/texture.h>
#include <opengl/shader.h>
#include <scene/scene.h>
#include <core/camera.h>
#include <renderers/renderlist.h>
class ForwardRenderer {
public:
  void init();

  void render(const Camera &camera, const Scene &scene);

  void render_sky(const Camera &camera);

  /**
   * @brief Render a single geometry with a single material
   *   mainly used to bypass the scene graph complexity to render something for
   * test.
   *
   * @param camera
   * @param program
   * @param material
   * @param geometry_buffer
   */
  void render_single(const Camera &camera, const GLProgram &program,
                     const Material &material,
                     const GeometryBuffer &geometry_buffer) {

    // program.set_uniform("model", GLUniform{mat4(1.0f)});
    // for (const auto &uniform : material.uniforms()) {
    //   program.set_uniform(uniform.first.c_str(), uniform.second);
    // }
    // auto &textures = material.textures();
    // for (int i = 0; i < material.textures().size(); i++) {
    //   auto texture = ResourceCache::instance().get<Texture>(texture_id);
    //   glActiveTexture(GL_TEXTURE0 + i);
    //   GLenum target = GL_TEXTURE_2D;
    //   glBindTexture(target, texture->id());
    // }

    geometry_buffer.draw();
  }

private:
  RenderList list;
  bool is_initialized{false};

  size_t MAX_LIGHT_COUNT = 10;

  std::unique_ptr<GeometryBuffer> cube_buffer;
  std::unique_ptr<GLTextureCube> skybox_texture;
  std::unique_ptr<GLProgram> skybox_program;
  Material skybox_material;

  std::unique_ptr<GLProgram> grid_program;
  GLuint grid_vao;
};