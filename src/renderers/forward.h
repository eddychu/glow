#pragma once
#include <scene/material.h>
#include <opengl/texture.h>
#include <opengl/shader.h>
#include <scene/scene.h>
#include <core/camera.h>
#include <renderers/renderlist.h>
class ForwardRenderer {
public:
  void render(const Camera &camera, const Scene &scene);

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
};