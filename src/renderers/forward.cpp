#include <renderers/renderlist.h>
#include <renderers/forward.h>
#include <scene/scene.h>
#include <opengl/shader.h>
void ForwardRenderer::render(const Camera &camera, const Scene &scene) {
  if (!is_initialized) {
    list.from_scene(scene);
    is_initialized = true;
  }
  const auto &cache = list.cache;

  for (const auto &item : list.items) {
    cache.programs[item.material]->use();
    cache.programs[item.material]->set_uniform("view", camera.view_matrix());
    cache.programs[item.material]->set_uniform("projection",
                                               camera.projection_matrix());
    cache.programs[item.material]->set_uniform("model",
                                               item.transform.matrix());
    cache.geometry_buffers[item.geometry_buffer_index]->draw();
  }
}

// void ForwardRenderer::render_renderable(const Camera &camera,
//                                         const Renderable &renderable) {
//   renderable.program->use();
//   for (auto &render_item : renderable.render_items) {
//     for (auto &pairs : render_item.material->uniforms()) {
//       auto &uniform = pairs.second;
//       auto &name = pairs.first;
//       renderable.program->set_uniform(name.c_str(), uniform);
//     }
//     for (int i = 0; i < render_item.material->textures().size(); i++) {
//       auto &texture_id = render_item.material->textures()[i];
//       auto texture = ResourceCache::instance().get<Texture>(texture_id);
//       glActiveTexture(GL_TEXTURE0 + i);
//       GLenum target = GL_TEXTURE_2D;
//       if (texture->type() == ResourceType::TextureCube) {
//         target = GL_TEXTURE_CUBE_MAP;
//       }
//       glBindTexture(target, texture->id());
//     }
//     render_item.geometry->draw();
//   }
// }