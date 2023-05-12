#pragma once
#include <resource/resource.h>
#include <glm/glm.hpp>
#include <stdint.h>

class Material : public Resource {
public:
  Material(const glm::vec3 &color = glm::vec3(1.0, 0.0, 0.0),
           float roughness = 0.5f, float metallic = 0.5f)
      : Resource(ResourceType::Material) {}
  void set_color(const glm::vec3 &color) { m_color = color; }
  const glm::vec3 &color() const { return m_color; }

  void set_roughness(float roughness) { m_roughness = roughness; }
  float roughness() const { return m_roughness; }

  void set_metallic(float metallic) { m_metallic = metallic; }
  float metallic() const { return m_metallic; }

  void set_shader_id(uint32_t shader_id) { m_shader_id = shader_id; }
  uint32_t shader_id() const { return m_shader_id; }

  void set_albedo_texture_id(uint32_t albedo_texture_id) {
    m_albedo_texture_id = albedo_texture_id;
  }

  uint32_t albedo_texture_id() const { return m_albedo_texture_id; }

  void set_normal_texture_id(uint32_t normal_texture_id) {
    m_normal_texture_id = normal_texture_id;
  }

  uint32_t normal_texture_id() const { return m_normal_texture_id; }

  void set_metallic_texture_id(uint32_t meta_texture_id) {
    m_meta_texture_id = meta_texture_id;
  }

  uint32_t metallic_texture_id() const { return m_meta_texture_id; }

  void set_ao_texture_id(uint32_t ao_texture_id) {
    m_ao_texture_id = ao_texture_id;
  }

  uint32_t ao_texture_id() const { return m_ao_texture_id; }

  void set_emissive_texture_id(uint32_t emissive_texture_id) {
    m_emissive_texture_id = emissive_texture_id;
  }

  uint32_t emissive_texture_id() const { return m_emissive_texture_id; }

private:
  glm::vec3 m_color{0.5f, 0.5f, 0.5f};
  float m_roughness{0.5f};
  float m_metallic{0.5f};

  uint32_t m_albedo_texture_id{0};
  uint32_t m_normal_texture_id{0};
  uint32_t m_meta_texture_id{0};
  uint32_t m_ao_texture_id{0};
  uint32_t m_emissive_texture_id{0};
  uint32_t m_shader_id{0};
};