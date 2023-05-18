#pragma once

#include <stdint.h>
#include <string>
extern uint32_t g_resource_id;
struct SceneResource {
  SceneResource() : m_id(next_id()) {}
  virtual ~SceneResource() = default;
  static uint32_t next_id() { return g_resource_id++; }
  uint32_t id() const { return m_id; }

protected:
  std::string m_name{""};
  uint32_t m_id;
};