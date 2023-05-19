#pragma once
#include <glm/glm.hpp>
#include <variant>

struct DirectionalLight {
  glm::vec3 direction;
  glm::vec3 color;
  float intensity;
};

struct PointLight {
  glm::vec3 position;
  glm::vec3 color;
  float intensity;
};

struct SpotLight {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec3 direction;
  float intensity;
  float cutoff; // cutoff angle in degrees
};

// struct SpotLight {
//   glm::vec3 position;
//   glm::vec3 direction;
//   glm::vec3 color;
//   float intensity;
//   float cut_off;
//   float outer_cut_off;
// };

typedef std::variant<DirectionalLight, PointLight, SpotLight> Light;