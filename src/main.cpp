#include "glm/ext/matrix_transform.hpp"
#include <core/camera.h>
#include <geometry/geometry.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <graphics/glbackground.h>
#include <core/input.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <graphics/glgeometry.h>
#include <graphics/glshader.h>
#include <vector>

static const int width = 640;
static const int height = 480;

static Camera camera({
    .position = glm::vec3(0.0f, 0.0f, 5.0f),
    .target = glm::vec3(0.0f, 0.0f, 0.0f),
    .up = glm::vec3(0.0f, 1.0f, 0.0f),
    .fov = glm::radians(45.0f),
    .aspect = (float)width / (float)height,
    .near = 0.1f,
    .far = 100.0f,
});

int main() {
  if (!glfwInit()) {
    printf("Error: Failed to initialize GLFW.\n");
    return -1;
  }
  // set hint
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const int width = 640;
  const int height = 480;

  GLFWwindow *window =
      glfwCreateWindow(width, height, "Hello World", NULL, NULL);

  if (!window) {
    printf("Error: Failed to create GLFW window.\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Error: Failed to initialize GLAD.\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }
  Geometry cube = Geometry::cube();
  GLGeometry geometry(cube.vertices, cube.indices);

  GLProgram program("assets/shaders/main.vert.glsl",
                    "assets/shaders/main.frag.glsl");

  glm::mat4 model = glm::mat4(1.0f);

  static double last_pos_x = 0.0;
  static double last_pos_y = 0.0;
  static bool mouse_holding = false;
  static vec3 eye = vec3(0.0f, 0.0f, 5.0f);
  static vec3 target = vec3(0.0f, 0.0f, 0.0f);
  static mat4 view_matrix = glm::lookAt(eye, target, vec3(0.0, 1.0, 0.0));

  glfwSetMouseButtonCallback(
      window, [](GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
          mouse_holding = true;
          glfwGetCursorPos(window, &last_pos_x, &last_pos_y);
        } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
          mouse_holding = false;
        }
      });

  glfwSetCursorPosCallback(
      window, [](GLFWwindow *window, double xpos, double ypos) {
        if (mouse_holding) {
          float x_offset = xpos - last_pos_x;
          float y_offset = ypos - last_pos_y;
          vec3 dist_vec = eye - target;
          float radius = glm::length(dist_vec);
          float theta = 0.0;
          float phi = 0.0;
          if (radius > 0.001) {
            theta = atan2(dist_vec.z, dist_vec.x);
            float v = min(1.0f, max(-1.0f, dist_vec.y / radius));
            phi = acos(v);
          }

          theta -= x_offset * glm::pi<float>() * 2.0 / height;
          phi -= y_offset * glm::pi<float>() * 2.0 / height;

          if (phi < 0.001) {
            phi = 0.001;
          } else if (phi > glm::pi<float>() - 0.001) {
            phi = glm::pi<float>() - 0.001;
          }

          float sin_phi = sin(phi) * radius;
          float new_x = sin_phi * sin(theta);
          float new_z = sin_phi * cos(theta);
          float new_y = cos(phi) * radius;
          eye = vec3(new_x, new_y, new_z);
          view_matrix = glm::lookAt(eye, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));

          // camera.set_position(vec3(new_x, new_y, new_z));
          last_pos_x = xpos;
          last_pos_y = ypos;
        }
      });

  glfwSetScrollCallback(
      window, [](GLFWwindow *window, double xoffset, double yoffset) {});

  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
                                int action, int mods) {

  });

  GLBackground background;

  glfwSwapInterval(1);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);
  glCullFace(GL_BACK);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.set_uniform("view", GLUniform{view_matrix});
    program.set_uniform("projection", GLUniform{camera.projection_matrix()});
    program.set_uniform("model", GLUniform{model});

    geometry.draw(program);
    // background.get_program().set_uniform("view",
    //                                      GLUniform{mat4(mat3(view_matrix))});
    // background.get_program().set_uniform("projection",
    //                                      GLUniform{camera.projection_matrix()});
    // background.draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  geometry.destroy();
  program.destroy();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
