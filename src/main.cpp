#include "opengl/texture.h"
#include <scene/geometry.h>
#include <opengl/geometry_buffer.h>
#include <scene/material.h>
#include <opengl/shader.h>
#include <renderers/forward.h>
#include <core/window.h>
#include <controllers/camera_controller.h>
#include <stb_image.h>
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
      .fov = 45.0f * glm::pi<float>() / 180.0f,
      .aspect = (float)width / (float)height,
      .near = 0.1f,
      .far = 100.0f,
  });

  CameraController controller(&camera, width, height);
  Geometry cube = make_cube(1.0);
  GeometryBuffer buffer(cube);
  Material material;

  ShaderSource source;
  source.vertex_path = "assets/shaders/basic.vert.glsl";
  source.fragment_path = "assets/shaders/basic.frag.glsl";

  material.set_shader_source(source);

  GLProgram program(material);

  Scene scene = load_scene("assets/helmet/DamagedHelmet.gltf");

  GLTexture texture(scene.textures[0]);

  ForwardRenderer renderer;

  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  while (!window.should_close()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // renderer.render_single(camera, program, material, buffer);
    // // renderer.render_single(camera, program, material, *geometry_buffer);
    renderer.render(camera, scene);
    window.swap_buffers();
    window.poll_events();
  }
  return 0;
}