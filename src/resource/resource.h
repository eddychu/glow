#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <memory>
enum class ResourceType {
  Texture2d,
  TextureCube,
  Material,
  Geometry,
  Shader,
  Unknown
};

extern uint32_t g_resource_id;

class Resource {
public:
  Resource(ResourceType type) : m_type(type), m_id(next_id()) {}

  virtual ~Resource() = default;

  static uint32_t next_id() { return g_resource_id++; }

  ResourceType type() const { return m_type; }
  std::string name() const { return m_name; }
  void set_type(ResourceType type) { m_type = type; }
  void set_name(const std::string &name) { m_name = name; }
  uint32_t id() const { return m_id; }

protected:
  ResourceType m_type = ResourceType::Unknown;
  std::string m_name{""};
  uint32_t m_id;
};

class ResourceCache {
public:
  ResourceCache() = default;
  ~ResourceCache() = default;

  template <typename T> T *get(uint32_t id) {
    auto it = m_resources.find(id);
    if (it != m_resources.end()) {
      return static_cast<T *>(it->second.get());
    }
    return nullptr;
  }

  template <typename T> T *get(const std::string &name) {
    for (auto &it : m_resources) {
      if (it.second->name() == name) {
        return static_cast<T *>(it.second.get());
      }
    }
    return nullptr;
  }

  template <typename T> void add(std::unique_ptr<T> resource) {
    m_resources[resource->id()] = std::move(resource);
  }

  void remove(uint32_t id) { m_resources.erase(id); }

private:
  std::unordered_map<uint32_t, std::unique_ptr<Resource>> m_resources;
};
