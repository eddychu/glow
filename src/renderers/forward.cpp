#include "glm/fwd.hpp"
#include <renderers/renderlist.h>
#include <renderers/forward.h>
#include <scene/scene.h>
#include <opengl/shader.h>
void ForwardRenderer::init() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ForwardRenderer::render(const Camera &camera, const Scene &scene) {
  if (!is_initialized) {
    list.from_scene(scene);
    is_initialized = true;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  const auto &cache = list.cache;
  for (const auto &item : list.items) {
    cache.programs[item.material]->use();
    cache.programs[item.material]->set_uniform("view", camera.view_matrix());
    cache.programs[item.material]->set_uniform("projection",
                                               camera.projection_matrix());
    cache.programs[item.material]->set_uniform("model",
                                               item.transform.matrix());
    cache.programs[item.material]->set_uniform("camera_pos",
                                               camera.transform().position());
    auto light_count = glm::min(scene.lights.size(), MAX_LIGHT_COUNT);
    cache.programs[item.material]->set_uniform("light_count", int(light_count));
    for (uint32_t i = 0; i < light_count; i++) {
      auto light = scene.lights[i];
      if (std::holds_alternative<DirectionalLight>(light)) {
        auto directional_light = std::get<DirectionalLight>(light);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].position", i).c_str(),
            directional_light.direction);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].color", i).c_str(),
            directional_light.color);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].intensity", i).c_str(),
            directional_light.intensity);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].is_directional", i).c_str(), true);
      } else if (std::holds_alternative<PointLight>(light)) {
        auto point_light = std::get<PointLight>(light);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].position", i).c_str(),
            point_light.position);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].color", i).c_str(), point_light.color);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].intensity", i).c_str(),
            point_light.intensity);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].is_directional", i).c_str(), false);
      }
    }
    auto &material = scene.materials[item.material];
    auto &textures = material.textures();
    for (uint32_t i = 0; i < textures.size(); i++) {
      cache.textures[textures[i]]->bind(i);
    }

    cache.geometry_buffers[item.geometry_buffer_index]->draw();
  }
}
