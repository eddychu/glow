#include <rendering/brdfpass.h>
#include <rendering/prefilterpass.h>
#include <rendering/pbrpass.h>
#include <rendering/skypass.h>
#include <resource/resource.h>
#include <resource/texture.h>
#include <spdlog/spdlog.h>
#include <resource/geometry_buffer.h>
#include <core/window.h>
#include <resource/shader.h>
#include <memory>
#include <resource/material.h>
#include <resource/scene.h>
#include <controllers/camera_controller.h>
#include <core/camera.h>
#include <light/light.h>
#include <stdint.h>
int main() {
  int width = 1024;
  int height = 768;
  Window window;

  window.initialize({
      .width = width,
      .height = height,
      .title = "Glow Renderer 0.1",
      .is_fullscreen = false,
      .multisample = 4,
  });

  Camera camera(CameraConfig{
      .position = vec3(0.0f, 0.0f, 3.0f),
      .target = vec3(0.0f),
      .up = vec3(0.0f, 1.0f, 0.0f),
      .fov = 45.0f,
      .aspect = (float)width / (float)height,
      .near = 0.1f,
      .far = 100.0f,
  });

  CameraController controller(&camera, width, height);

  std::vector<Light> lights = {
      Light{.value =
                DirectionalLight{
                    .direction = vec3(0.0f, 1.0f, 0.0f),
                    .color = vec3(1.0f),
                    .intensity = 1.f,
                }},
      Light{.value =
                DirectionalLight{
                    .direction = vec3(0.0f, -1.0f, 0.0f),
                    .color = vec3(1.0f),
                    .intensity = 1.f,
                }},
      Light{.value =
                PointLight{
                    .position = vec3(0.0f, 0.0f, 5.0f),
                    .color = vec3(1.0f),
                    .intensity = 1.f,
                }}

  };

  ResourceCache cache;
  uint32_t scene_id = load_scene("assets/helmet/DamagedHelmet.gltf", &cache);
  auto pbr_shader = std::make_unique<GLProgram>(
      "assets/shaders/simple.vert.glsl", "assets/shaders/simple.frag.glsl");
  auto pbr_shader_id = pbr_shader->id();
  cache.add(std::move(pbr_shader));

  auto skybox_shader =
      std::make_unique<GLProgram>("assets/shaders/background.vert.glsl",
                                  "assets/shaders/background.frag.glsl");

  auto skybox_shader_id = skybox_shader->id();
  cache.add(std::move(skybox_shader));

  double last_pos_x = 0.0;
  double last_pos_y = 0.0;
  window.register_on_mouse_button_func([&](int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
      controller.set_enable(true);
      auto cursor_pos = window.get_cursor_pos();
      last_pos_x = cursor_pos[0];
      last_pos_y = cursor_pos[1];
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
      controller.set_enable(false);
    }
  });

  window.register_on_cursor_pos_func([&](double xpos, double ypos) {
    float x_offset = xpos - last_pos_x;
    float y_offset = ypos - last_pos_y;
    controller.rotate(x_offset, y_offset);
    // view = camera.view_matrix();
    last_pos_x = xpos;
    last_pos_y = ypos;
  });

  window.register_on_window_size_func([&](int new_width, int new_height) {
    width = new_width;
    height = new_height;
    camera.set_aspect((float)width / (float)height);
    // todo: update camera projection matrix
    // projection = camera.projection_matrix();
  });

  // auto scene = cache.get<Scene>(scene_id);
  std::vector<PBRPass> passes = from_scene(scene_id, &cache);

  auto texture_cube =
      std::make_unique<TextureCube>("assets/skybox/grace", "hdr");
  auto texture_cube_id = texture_cube->id();
  cache.add(std::move(texture_cube));

  auto irradiance_map =
      std::make_unique<TextureCube>("assets/skybox/grace-diffuse", "hdr");
  auto irradiance_map_id = irradiance_map->id();
  cache.add(std::move(irradiance_map));

  SkyPass sky_pass;
  auto background_shader = cache.get<GLProgram>(skybox_shader_id);
  auto skybox_texture = cache.get<TextureCube>(texture_cube_id);
  sky_pass.init(background_shader, skybox_texture, &camera);

  PrefilterPass prefilter_pass;

  auto prefilter_shader =
      std::make_unique<GLProgram>("assets/shaders/background.vert.glsl",
                                  "assets/shaders/prefilter.frag.glsl");
  auto prefilter_shader_id = prefilter_shader->id();
  cache.add(std::move(prefilter_shader));
  prefilter_pass.init(cache.get<GLProgram>(prefilter_shader_id),
                      cache.get<TextureCube>(texture_cube_id));

  auto brdf_shader = std::make_unique<GLProgram>(
      "assets/shaders/brdf.vert.glsl", "assets/shaders/brdf.frag.glsl");
  auto brdf_shader_id = brdf_shader->id();
  cache.add(std::move(brdf_shader));

  BRDFPass brdf_pass;
  brdf_pass.init(cache.get<GLProgram>(brdf_shader_id),
                 prefilter_pass.capture_fbo, prefilter_pass.capture_rbo);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CCW);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  prefilter_pass.render();
  brdf_pass.render();

  while (!window.should_close()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto &pass : passes) {
      pass.samples = 4;
      pass.camera = &camera;
      pass.width = width;
      pass.height = height;
      pass.program = cache.get<GLProgram>(pbr_shader_id);
      pass.irradiance_texture = cache.get<TextureCube>(irradiance_map_id);
      pass.brdf_lut = brdf_pass.brdf_texture;
      pass.prefilter_map = prefilter_pass.prefilter_texture;
      pass.lights = lights.data();
      pass.light_count = lights.size();
      pass.render();
    }

    sky_pass.render();
    window.swap_buffers();
    window.poll_events();
  }

  return 0;
}