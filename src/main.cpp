#include <geometry/geometry.h>
#include <resource/geometry_buffer.h>
#include <scene/material.h>
#include <resource/shader.h>
#include "resource/resource.h"
#include <renderers/forward.h>
#include <core/window.h>
#include <controllers/camera_controller.h>
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
      .position = vec3(0.0f, 0.0f, 10.0f),
      .target = vec3(0.0f),
      .up = vec3(0.0f, 1.0f, 0.0f),
      .fov = 45.0f * glm::pi<float>() / 180.0f,
      .aspect = (float)width / (float)height,
      .near = 0.1f,
      .far = 100.0f,
  });

  CameraController controller(&camera, width, height);
  Geometry cube = make_cube(1.0);
  GeometryBuffer buffer(cube.vertices, cube.indices);
  Material material;

  GLProgram program("assets/shaders/basic.vert.glsl",
                    "assets/shaders/basic.frag.glsl");

  // Scene scene = load_scene("assets/helmet/DamagedHelmet.gltf");
  Scene scene = load_scene("assets/Sponza/glTF/Sponza.gltf");
  ForwardRenderer renderer;

  // auto geometry_buffer = ResourceCache::instance().get<GeometryBuffer>(
  //     scene.meshes[0].sub_meshes[80].geometry_id);
  glEnable(GL_DEPTH_TEST);

  while (!window.should_close()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.use();
    program.set_uniform("model", GLUniform{scene.nodes[0].transform.matrix()});
    program.set_uniform("view", GLUniform{camera.view_matrix()});
    program.set_uniform("projection", GLUniform{camera.projection_matrix()});
    for (auto &submesh : scene.meshes[0].sub_meshes) {
      renderer.render_single(
          camera, program, material,
          *ResourceCache::instance().get<GeometryBuffer>(submesh.geometry_id));
    }
    // renderer.render_single(camera, program, material, *geometry_buffer);
    window.swap_buffers();
    window.poll_events();
  }

  return 0;
}