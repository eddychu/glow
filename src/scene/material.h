#pragma once
#include <resource/shader.h>
#include <string>
#include <glm/glm.hpp>
#include <stdint.h>
#include <unordered_map>
#include <variant>

class Material {
public:
  void set_shader(uint32_t shader_id) { m_shader_id = shader_id; }

  uint32_t shader_id() const { return m_shader_id; }

  void add_uniform(const std::string &name, const GLUniform &uniform) {
    m_uniforms[name] = uniform;
  }

  void add_texture(uint32_t texture_id) { m_textures.push_back(texture_id); }

  const std::unordered_map<std::string, GLUniform> &uniforms() const {
    return m_uniforms;
  }

  const std::vector<uint32_t> &textures() const { return m_textures; }

private:
  std::unordered_map<std::string, GLUniform> m_uniforms;
  std::vector<uint32_t> m_textures;
  uint32_t m_shader_id;
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
