// #include <core/renderer.h>
// #include <graphics/glframebuffer.h>
// #include <graphics/gltexture.h>
// #include <light/light.h>
// #include <core/camera.h>
// #include <geometry/geometry.h>
// #include <glm/glm.hpp>
// #include <glm/ext.hpp>
// #include <graphics/glbackground.h>
// #include <core/input.h>
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <cstdio>
// #include <graphics/glgeometry.h>
// #include <graphics/glshader.h>
// #include <vector>
// #include <graphics/glrenderer.h>
// #include <core/window.h>
// #include <controllers/camera_controller.h>
// static int width = 640;
// static int height = 480;

// int main() {
//   Window window;
//   window.initialize({
//       .width = width,
//       .height = height,
//       .title = "OpenGL",
//       .is_fullscreen = false,
//   });

//   Camera camera({
//       .position = glm::vec3(0.0f, 4.0f, 5.0f),
//       .target = glm::vec3(0.0f, 0.0f, 0.0f),
//       .up = glm::vec3(0.0f, 1.0f, 0.0f),
//       .fov = glm::radians(45.0f),
//       .aspect = (float)width / (float)height,
//       .near = 0.1f,
//       .far = 100.0f,
//   });
//   CameraController controller(&camera, width, height);
//   Geometry cube = Geometry::cube();
//   Geometry plane = Geometry::quad(10.0f);
//   Geometry quad = Geometry::quad(1.0f);
//   GLGeometry cube_geometry(cube.vertices, cube.indices);
//   GLGeometry plane_geometry(plane.vertices, plane.indices);
//   // GLGeometry quad_geometry(quad.vertices, quad.indices);
//   vec3 light_pos = vec3(-3.0f, 5.0f, 0.0f);
//   Light light = {
//       .direction = glm::normalize(light_pos - glm::vec3(0.0, 0.0, 0.0)),
//       .color = glm::vec3(1.0f, 1.0f, 1.0f),
//       .intensity = 1.0f,
//   };

//   mat4 plane_transform =
//       glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -0.5, 0.0));

//   GLProgram program("assets/shaders/main.vert.glsl",
//                     "assets/shaders/main.frag.glsl");

//   GLProgram shadow_program("assets/shaders/shadow.vert.glsl",
//                            "assets/shaders/shadow.frag.glsl");

//   glm::mat4 model = glm::mat4(1.0f);

//   mat4 light_proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
//   mat4 light_view = glm::lookAt(light_pos, vec3(0.0f), vec3(0.0f, 1.0f,
//   0.0f)); mat4 shadow_matrix = light_proj * light_view;

//   double last_pos_x = 0.0;
//   double last_pos_y = 0.0;
//   window.register_on_mouse_button_func([&](int button, int action, int mods)
//   {
//     if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//       controller.set_enable(true);
//       auto cursor_pos = window.get_cursor_pos();
//       last_pos_x = cursor_pos[0];
//       last_pos_y = cursor_pos[1];
//     } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
//       controller.set_enable(false);
//     }
//   });

//   window.register_on_cursor_pos_func([&](double xpos, double ypos) {
//     float x_offset = xpos - last_pos_x;
//     float y_offset = ypos - last_pos_y;
//     controller.rotate(x_offset, y_offset);
//     last_pos_x = xpos;
//     last_pos_y = ypos;
//   });

//   window.register_on_window_size_func([&](int new_width, int new_height) {
//     width = new_width;
//     height = new_height;
//     camera.set_aspect((float)width / (float)height);
//     // todo: update camera projection matrix
//     // projection_matrix = glm::perspective(
//     //     glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
//   });

//   window.register_on_key_func([&](int key, int scancode, int action, int
//   mods) {
//     if (key == GLFW_KEY_W) {
//       light_pos.z -= 0.1f;
//     }
//     if (key == GLFW_KEY_S) {
//       light_pos.z += 0.1f;
//     }

//     light.direction = glm::normalize(light_pos - glm::vec3(0.0, 0.0, 0.0));
//     light_view = glm::lookAt(light_pos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
//     shadow_matrix = light_proj * light_view;
//   });

//   GLBackground background;

//   GLTextureConfig depth_texture_config = {
//       .target = GL_TEXTURE_2D,
//       .width = 1024,
//       .height = 1024,
//       .format = GL_DEPTH_COMPONENT24,
//       .wrap_s = GL_CLAMP_TO_BORDER,
//       .wrap_t = GL_CLAMP_TO_BORDER,
//       .min_filter = GL_NEAREST,
//       .mag_filter = GL_NEAREST,
//       .border_color = {1.0f, 1.0f, 1.0f, 1.0f},
//   };

//   GLTexture depth_texture(depth_texture_config);
//   GLFramebuffer shadow_framebuffer(depth_texture_config.width,
//                                    depth_texture_config.height);

//   shadow_framebuffer.attach(depth_texture, GL_DEPTH_ATTACHMENT, 0);

//   if (shadow_framebuffer.is_complete()) {
//     printf("Framebuffer is complete.\n");
//   } else {
//     printf("Framebuffer is not complete.\n");
//   }

//   // glfwSwapInterval(1);
//   // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//   // glEnable(GL_DEPTH_TEST);
//   // glEnable(GL_CULL_FACE);
//   // glDepthFunc(GL_LEQUAL);
//   // glCullFace(GL_BACK);

//   GLRenderer renderer(width, height);
//   renderer.initialize();

//   while (!window.should_close()) {
//     renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     // the shadow pass
//     // glCullFace(GL_FRONT);
//     // glViewport(0, 0, 1024, 1024);
//     // glBindFramebuffer(GL_FRAMEBUFFER, shadow_framebuffer.handle());
//     shadow_framebuffer.bind();
//     glClear(GL_DEPTH_BUFFER_BIT);
//     shadow_program.set_uniform("shadow", GLUniform{shadow_matrix});
//     shadow_program.set_uniform("model", GLUniform{model});
//     cube_geometry.draw(shadow_program);

//     shadow_program.set_uniform("model", GLUniform{plane_transform});

//     plane_geometry.draw(shadow_program);
//     shadow_framebuffer.unbind();

//     // glCullFace(GL_BACK);
//     glViewport(0, 0, width, height);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     program.use();
//     depth_texture.bind(0);
//     program.set_uniform("view_pos",
//     GLUniform{camera.transform().position()});
//     program.set_uniform("light_space_matrix", GLUniform{shadow_matrix});
//     program.set_uniform("view", GLUniform{camera.view_matrix()});
//     program.set_uniform("projection", GLUniform{camera.projection_matrix()});
//     program.set_uniform("model", GLUniform{model});
//     program.set_uniform("light.direction", GLUniform{light.direction});
//     program.set_uniform("light.color", GLUniform{light.color});
//     program.set_uniform("light.intensity", GLUniform{light.intensity});
//     cube_geometry.draw(program);
//     program.set_uniform("model", GLUniform{plane_transform});
//     plane_geometry.draw(program);
//     background.get_program().set_uniform(
//         "view", GLUniform{mat4(mat3(camera.view_matrix()))});
//     background.get_program().set_uniform("projection",
//                                          GLUniform{camera.projection_matrix()});
//     background.draw();
//     window.swap_buffers();
//     glfwPollEvents();
//   }
//   // background.destroy();
//   cube_geometry.destroy();
//   plane_geometry.destroy();
//   program.destroy();
//   window.destroy();
//   return 0;
// }

#include "spdlog/spdlog.h"
#include <resource/geometry.h>
#include <core/window.h>
#include <resource/shader.h>
#include <memory>
#include <resource/material.h>
#include <scene/scene.h>
int main() {
  int width = 1024;
  int height = 768;
  Window window;

  window.initialize({
      .width = width,
      .height = height,
      .title = "Hello World",
      .is_fullscreen = false,
  });

  ResourceCache cache;
  Scene scene = load_scene("assets/helmet/DamagedHelmet.gltf", &cache);
  auto shader = std::make_unique<GLProgram>("assets/shaders/simple.vert.glsl",
                                            "assets/shaders/simple.frag.glsl");
  auto shader_id = shader->id();
  cache.add(std::move(shader));

  glm::mat4 view =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

  while (!window.should_close()) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto &node : scene.nodes) {
      // update
      // node->update();
      // draw
      if (node.mesh > -1) {
        auto mesh = scene.meshes[node.mesh];
        for (const auto &sub_mesh : mesh.sub_meshes) {
          auto material_id = sub_mesh.material_id;
          auto geometry_id = sub_mesh.geometry_id;
          auto material = cache.get<Material>(material_id);
          auto program = cache.get<GLProgram>(shader_id);
          auto geometry = cache.get<Geometry>(geometry_id);
          program->use();
          spdlog::info("Drawing mesh {} with material {} and geometry {}",
                       node.mesh, material_id, geometry_id);
          program->set_uniform("model", GLUniform{glm::mat4(1.0f)});
          program->set_uniform("view", GLUniform{view});
          program->set_uniform("projection", GLUniform{projection});
          glBindVertexArray(geometry->vao);
          if (geometry->has_indices) {
            glDrawElements(GL_TRIANGLES, geometry->count, GL_UNSIGNED_SHORT, 0);
          } else {
            glDrawArrays(GL_TRIANGLES, 0, geometry->count);
          }
        }
      }
    }

    window.swap_buffers();
    window.poll_events();
  }

  return 0;
}