#include <light/light.h>
#include <core/window.h>
#include <scene/geometry.h>
#include <opengl/geometry_buffer.h>
#include <scene/material.h>
#include <opengl/shader.h>
#include <renderers/forward.h>
#include <controllers/camera_controller.h>
#include <loaders/texture_loader.h>
int main() {
  int width = 1024;
  int height = 768;
  Window window;

  window.initialize({
      .width = width,
      .height = height,
      .title = "Glow Renderer 0.1",
      .is_fullscreen = false,
      .multisample = 16,
  });

  Camera camera(CameraConfig{
      .position = vec3(0, 0.0, 5.0f),
      .target = vec3(0.0f),
      .up = vec3(0.0f, 1.0f, 0.0f),
      .fov = 45.0f * glm::pi<float>() / 180.0f,
      .aspect = (float)width / (float)height,
      .near = 0.1f,
      .far = 100.0f,
  });

  CameraController controller(&camera, &window);
  Scene scene = load_scene("assets/helmet/DamagedHelmet.gltf");
  // Scene scene = load_scene("assets/Sponza/glTF/Sponza.gltf");
  // Scene scene = load_scene("assets/plane/glTF/TwoSidedPlane.gltf");
  std::vector<Texture> textures(6);
  std::vector<std::string> paths = {
      "assets/skybox/pisa/posx.png", "assets/skybox/pisa/negx.png",
      "assets/skybox/pisa/posy.png", "assets/skybox/pisa/negy.png",
      "assets/skybox/pisa/posz.png", "assets/skybox/pisa/negz.png",
  };
  auto environment = load_texture_cube(paths);
  scene.environment = std::move(environment);
  scene.lights = {
      // DirectionalLight{
      //     .direction = vec3(0.0f, 0.0f, 1.0f),
      //     .color = vec3(1.0f),
      //     .intensity = 1.0f,
      // },

      PointLight {
        .position = vec3(0.0f, 3.0f, 0.0f),
        .color = vec3(1.0f),
        .intensity = 1.0f,
      },

      // SpotLight {
      //   .position = vec3(0.0f, 5.0f, 0.0f),
      //   .color = vec3(1.0f),
      //   .direction = vec3(0.0f, -1.0f, 0.0f),
      //   .intensity = 1.0f,
      //   .cutoff = 0.1f,
      // }
  };

  camera.set_target(scene.nodes[0].bbox.center());

  // scene.nodes[0].transform.set_scale(vec3(10.0f));
  ForwardRenderer renderer;
  renderer.init();
  while (!window.should_close()) {
    renderer.render(camera, scene);
    window.swap_buffers();
    window.poll_events();
  }
  return 0;
}