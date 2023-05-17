#pragma once
#include <string>
#include <glm/glm.hpp>
#include <stdint.h>
#include <unordered_map>
#include <variant>

struct ShaderSource {
  std::string vertex_path;
  std::string fragment_path;

  // override equal operator
  bool operator==(const ShaderSource &other) const {
    return vertex_path == other.vertex_path &&
           fragment_path == other.fragment_path;
  }
};

extern ShaderSource default_shader_source;

extern uint32_t g_material_id;

class Material {
public:
  Material()
      : m_shader_source(default_shader_source), m_id(next_material_id()) {}

  static uint32_t next_material_id() { return g_material_id++; }

  uint32_t id() const { return m_id; }

  const ShaderSource &shader_source() const { return m_shader_source; }

  void set_shader_source(const ShaderSource &shader_source) {
    m_shader_source = shader_source;
  }

  void add_texture(const std::string &name, uint32_t texture_id) {
    m_textures[name] = texture_id;
  }

  const std::unordered_map<std::string, uint32_t> &textures() const {
    return m_textures;
  }

private:
  std::unordered_map<std::string, uint32_t> m_textures;
  ShaderSource m_shader_source = default_shader_source;
  uint32_t m_id;
};
// class PBRMaterial {
// public:
//   PBRMaterial() = default;

//   void set_shader_id(uint32_t shader_id) { m_shader_id = shader_id; }
//   uint32_t shader_id() const { return m_shader_id; }

//   void set_albedo_texture_id(uint32_t albedo_texture_id) {
//     m_albedo_texture_id = albedo_texture_id;
//   }

//   uint32_t albedo_texture_id() const { return m_albedo_texture_id; }

//   void set_normal_texture_id(uint32_t normal_texture_id) {
//     m_normal_texture_id = normal_texture_id;
//   }

//   uint32_t normal_texture_id() const { return m_normal_texture_id; }

//   void set_metallic_texture_id(uint32_t meta_texture_id) {
//     m_meta_texture_id = meta_texture_id;
//   }

//   uint32_t metallic_texture_id() const { return m_meta_texture_id; }

//   void set_ao_texture_id(uint32_t ao_texture_id) {
//     m_ao_texture_id = ao_texture_id;
//   }

//   uint32_t ao_texture_id() const { return m_ao_texture_id; }

//   void set_emissive_texture_id(uint32_t emissive_texture_id) {
//     m_emissive_texture_id = emissive_texture_id;
//   }

//   uint32_t emissive_texture_id() const { return m_emissive_texture_id; }

// private:
//   uint32_t m_albedo_texture_id{0};
//   uint32_t m_normal_texture_id{0};
//   uint32_t m_meta_texture_id{0};
//   uint32_t m_ao_texture_id{0};
//   uint32_t m_emissive_texture_id{0};
//   uint32_t m_shader_id{0};
// };
