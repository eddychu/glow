#pragma once
#include "core/transform.h"
#include <memory>
#include <opengl/texture.h>
#include <opengl/geometry_buffer.h>
#include <scene/material.h>
#include <opengl/resource.h>
#include <opengl/shader.h>
#include <scene/scene.h>
#include <stdint.h>
#include <unordered_map>
#include <vector>
/**
 * @brief A render item is a combination of a geometry buffer and a material.
 *
 */
struct RenderCache {
  std::vector<std::unique_ptr<GLProgram>> programs;
  std::vector<std::unique_ptr<GLTexture>> textures;
  std::vector<std::unique_ptr<GeometryBuffer>> geometry_buffers;
};

struct RenderItem {
  uint32_t geometry_buffer_index;
  uint32_t material;
  Transform transform;
};

/**
 * @brief A render list is a collection of items.
 */
struct RenderList {
  RenderCache cache;
  std::vector<RenderItem> items;
  void add_renderable(const RenderItem &renderable) {
    items.push_back(renderable);
  }

  const GLProgram &program(uint32_t index) const {
    return *cache.programs[index];
  }

  void from_scene(const Scene &scene) {
    for (uint32_t i = 0; i < scene.textures.size(); i++) {
      const auto &texture = scene.textures[i];
      cache.textures.push_back(std::make_unique<GLTexture>(texture));
    }
    for (uint32_t i = 0; i < scene.materials.size(); i++) {
      const auto &material = scene.materials[i];
      auto program = std::make_unique<GLProgram>(material);
      cache.programs.push_back(std::move(program));
    }
    for (int i = 0; i < scene.nodes.size(); i++) {
      const auto &node = scene.nodes[i];
      const auto &transform = node.transform;
      if (node.mesh > -1) {
        auto sub_meshes = scene.meshes[node.mesh].sub_meshes;
        for (const auto &sub_mesh : sub_meshes) {
          auto geometry_buffer =
              std::make_unique<GeometryBuffer>(sub_mesh.geometry);
          auto geometry_index = cache.geometry_buffers.size();
          cache.geometry_buffers.push_back(std::move(geometry_buffer));
          RenderItem render_item;
          render_item.material = sub_mesh.material;

          render_item.geometry_buffer_index = geometry_index;
          render_item.transform = transform;
          add_renderable(render_item);
        }
      }
    }
  }
};