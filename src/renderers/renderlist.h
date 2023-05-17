#pragma once

#include <opengl/geometry_buffer.h>
#include <scene/material.h>
#include <opengl/resource.h>
#include <opengl/shader.h>
#include <scene/scene.h>
#include <unordered_map>
#include <vector>

/**
 * @brief A render item is a combination of a geometry buffer and a material.
 *
 */
struct RenderItem {
  GeometryBuffer *geometry;
  Material *material;
  SubMesh *sub_mesh{nullptr};
};

/**
 * @brief A renderable is a collection of render items that share the same
 * shader.
 */
struct Renderable {
  std::vector<RenderItem> render_items;
  GLProgram *program;
  uint32_t material_id;
};

/**
 * @brief A render list is a collection of renderables.
 */
struct RenderList {
  std::vector<Renderable> renderables;
  void add_renderable(const Renderable &renderable) {
    renderables.push_back(renderable);
  }

  void from_scene(const Scene &scene) {
    for (int i = 0; i < scene.nodes.size(); i++) {
      const auto &node = scene.nodes[i];
      const auto &transform = node.transform;
      if (node.mesh > -1) {
        auto sub_meshes = scene.meshes[node.mesh].sub_meshes;
        for (const auto &sub_mesh : sub_meshes) {
          auto geometry_buffer = new GeometryBuffer(sub_mesh.geometry.vertices,
                                                    sub_mesh.geometry.indices);
          auto material = scene.materials[sub_mesh.material];
          for (auto &renderable : renderables) {
            if (renderable.material_id == material.id()) {
              renderable.render_items.push_back({geometry_buffer, &material});
            }
          }
          Renderable renderable;
          renderable.render_items.push_back({geometry_buffer, &material});
          renderable.material_id = material.id();
          const auto &shader_source = material.shader_source();
          renderable.program =
              new GLProgram(shader_source.vertex_path.c_str(),
                            shader_source.fragment_path.c_str());
          add_renderable(renderable);
        }
      }
    }
  }
};