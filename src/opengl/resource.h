#pragma once
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <memory>

class Resource {
public:
  virtual ~Resource() = default;
  uint32_t id() const { return m_id; }
  void set_id(uint32_t id) { m_id = id; }

protected:
  uint32_t m_id = {0};
};

class ResourceCache {
public:
  ResourceCache() = default;
  ~ResourceCache() = default;

  static ResourceCache &instance() {
    static ResourceCache cache;
    return cache;
  }

  template <typename T> T *get(uint32_t id) {
    auto it = m_resources.find(id);
    if (it != m_resources.end()) {
      return static_cast<T *>(it->second.get());
    }
    return nullptr;
  }

  // template <typename T> T *get(const std::string &name) {
  //   for (auto &it : m_resources) {
  //     if (it.second->name() == name) {
  //       return static_cast<T *>(it.second.get());
  //     }
  //   }
  //   return nullptr;
  // }

  template <typename T> uint32_t add(std::unique_ptr<T> resource) {
    uint32_t id = resource->id();
    m_resources[id] = std::move(resource);
    return id;
  }

  void remove(uint32_t id) { m_resources.erase(id); }

private:
  std::unordered_map<uint32_t, std::unique_ptr<Resource>> m_resources;
};
