#include <core/bbox.h>
#include <core/camera.h>
#include <opengl/geometry_buffer.h>
#include <scene/geometry.h>
#include <memory>
#include <renderers/renderlist.h>
#include <renderers/forward.h>
#include <scene/scene.h>
#include <opengl/shader.h>
#include <variant>
void ForwardRenderer::init() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  auto cube = make_cube(2.0f);
  cube_buffer = std::make_unique<GeometryBuffer>(cube);
  grid_program = std::make_unique<GLProgram>("assets/shaders/grid.vert.glsl",
                                             "assets/shaders/grid.frag.glsl");
  glCreateVertexArrays(1, &grid_vao);

  auto axis = make_axis(5.0f);
  axis_buffer = std::make_unique<GeometryBuffer>(axis);

  axis_program = std::make_unique<GLProgram>("assets/shaders/axis.vert.glsl",
                                             "assets/shaders/axis.frag.glsl");                                           
}

void ForwardRenderer::render(const Camera &camera, const Scene &scene) {
  if (!is_initialized) {
    list.from_scene(scene);
    is_initialized = true;
    if (scene.environment) {

      // TODO: this is a hack to get the skybox working
      skybox_texture = std::make_unique<GLTextureCube>(*scene.environment.get());
      skybox_material.set_shader_source(
          ShaderSource{.vertex_path = "assets/shaders/background.vert.glsl",
                       .fragment_path = "assets/shaders/background.frag.glsl"});
      skybox_program = std::make_unique<GLProgram>(skybox_material);
    }
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
            fmt::format("lights[{}].type", i).c_str(), 0);
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
            fmt::format("lights[{}].type", i).c_str(), 1);
      } else if (std::holds_alternative<SpotLight>(light)) {
        auto spot_light = std::get<SpotLight>(light);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].position", i).c_str(), spot_light.position);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].color", i).c_str(),
            spot_light.color);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].intensity", i).c_str(),
            spot_light.intensity);
        cache.programs[item.material]->set_uniform(
        fmt::format("lights[{}].direction", i).c_str(),
        spot_light.direction);
        cache.programs[item.material]->set_uniform(
            fmt::format("lights[{}].type", i).c_str(), 2);
            cache.programs[item.material]->set_uniform(fmt::format("lights[{}].cut_off", i).c_str(), spot_light.cutoff);
      }
    }
    auto &material = scene.materials[item.material];
    auto &textures = material.textures();
    for (uint32_t i = 0; i < textures.size(); i++) {
      cache.textures[textures[i]]->bind(i);
    }

    cache.geometry_buffers[item.geometry_buffer_index]->draw();

    // axis_program->use();
    // axis_program->set_uniform("view", camera.view_matrix());
    // axis_program->set_uniform("projection", camera.projection_matrix());
    // axis_program->set_uniform("model", item.transform.matrix());
    // cache.bbox_buffers[item.geometry_buffer_index]->draw();
  }

  // grid_program->use();
  // grid_program->set_uniform("view", camera.view_matrix());
  // grid_program->set_uniform("proj", camera.projection_matrix());
  // grid_program->set_uniform("cameraPos", camera.transform().position());
  // glBindVertexArray(grid_vao);
  // glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 6, 1, 0);

  // set line width
  // glLineWidth(2.0f);
  // axis_program->use();
  // axis_program->set_uniform("view", camera.view_matrix());
  // axis_program->set_uniform("projection", camera.projection_matrix());
  // axis_program->set_uniform("model", mat4(1.0f));
  // axis_buffer->draw();
  // glLineWidth(1.0f);

  // if (!scene.environment.empty()) {
  //   render_sky(camera);
  // }
}

void ForwardRenderer::render_sky(const Camera &camera) {
  skybox_program->use();
  skybox_program->set_uniform("view",
                              glm::mat4(glm::mat3(camera.view_matrix())));
  skybox_program->set_uniform("projection", camera.projection_matrix());
  skybox_texture->bind(0);
  cube_buffer->draw();
}