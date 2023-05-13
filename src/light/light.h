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
  glm::vec3 direction;
  glm::vec3 color;
  float intensity;
  float constant;
  float linear;
  float quadratic;
  float cut_off;
  float outer_cut_off;
};

struct Light {
  std::variant<DirectionalLight, PointLight, SpotLight> value;
};