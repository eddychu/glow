#pragma once
#include "scene/material.h"
#include "resource/texture.h"
#include "resource/shader.h"
#include "scene/scene.h"
#include <core/camera.h>
#include <renderers/renderlist.h>
class ForwardRenderer {
public:
  void render(const Camera &camera, const Scene &scene);
  void render_renderable(const Camera &camera, const Renderable &renderable);

  void render_single(const Camera &camera, const GLProgram &program,
                     const Material &material,
                     const GeometryBuffer &geometry_buffer) {
    program.use();
    program.set_uniform("view", GLUniform{camera.view_matrix()});
    program.set_uniform("projection", GLUniform{camera.projection_matrix()});
    program.set_uniform("model", GLUniform{mat4(1.0f)});
    for (const auto &uniform : material.uniforms()) {
      program.set_uniform(uniform.first.c_str(), uniform.second);
    }
    for (int i = 0; i < material.textures().size(); i++) {
      auto &texture_id = material.textures()[i];
      auto texture = ResourceCache::instance().get<Texture>(texture_id);
      glActiveTexture(GL_TEXTURE0 + i);
      GLenum target = GL_TEXTURE_2D;
      if (texture->type() == ResourceType::TextureCube) {
        target = GL_TEXTURE_CUBE_MAP;
      }
      glBindTexture(target, texture->id());
    }
    geometry_buffer.draw();
  }

private:
  RenderList list;
  bool is_initialized{false};
};