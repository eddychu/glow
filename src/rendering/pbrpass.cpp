#include <light/light.h>
#include <resource/material.h>
#include <resource/scene.h>
#include <rendering/pbrpass.h>
#include <stdint.h>
#include <unordered_map>

std::vector<PBRPass> from_scene(uint32_t scene_id, ResourceCache *cache) {

  auto scene = cache->get<Scene>(scene_id);
  // filter all geometries
  std::vector<PBRPass> passes(scene->materials.size());
  for (const auto &node : scene->nodes) {
    const auto &transform = node.transform;
    if (node.mesh > -1) {
      auto mesh = scene->meshes[node.mesh];
      auto materials = scene->materials;
      for (const auto &sub_mesh : mesh.sub_meshes) {
        auto geometry = cache->get<Geometry>(sub_mesh.geometry_id);
        auto &material = materials[sub_mesh.material];
        PBRPass pass;
        pass.geometries.push_back(geometry);
        pass.albedo_texture = cache->get<Texture>(material.albedo_texture_id());
        pass.normal_texture = cache->get<Texture>(material.normal_texture_id());
        pass.metallic_texture =
            cache->get<Texture>(material.metallic_texture_id());
        pass.ao_texture = cache->get<Texture>(material.ao_texture_id());
        pass.emissive_texture =
            cache->get<Texture>(material.emissive_texture_id());
        passes[sub_mesh.material] = pass;
      }
    }
  }
  return passes;
}

void PBRPass::render() {
  glViewport(0, 0, width, height);
  program->use();
  for (const auto &geometry : geometries) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo_texture->handle());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal_texture->handle());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallic_texture->handle());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ao_texture->handle());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, emissive_texture->handle());
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_texture->handle());
    for (int i = 0; i < light_count; ++i) {
      std::string light_name = "lights[" + std::to_string(i) + "]";
      auto value = lights[i].value;
      if (const auto *light = std::get_if<DirectionalLight>(&value); light) {
        program->set_uniform((light_name + ".position").c_str(),
                             GLUniform{light->direction});
        program->set_uniform((light_name + ".color").c_str(),
                             GLUniform{light->color});
        program->set_uniform((light_name + ".intensity").c_str(),
                             GLUniform{light->intensity});
        program->set_uniform((light_name + ".is_directional").c_str(),
                             GLUniform{1});
      } else if (const auto *light = std::get_if<PointLight>(&value); light) {
        program->set_uniform((light_name + ".position").c_str(),
                             GLUniform{light->position});
        program->set_uniform((light_name + ".color").c_str(),
                             GLUniform{light->color});
        program->set_uniform((light_name + ".intensity").c_str(),
                             GLUniform{light->intensity});
        program->set_uniform((light_name + ".is_directional").c_str(),
                             GLUniform{0});
      } else {
        spdlog::error("Unsupported light type");
        throw std::runtime_error("Unsupported light type");
      }
      //   try {
      //     DirectionalLight light = std::get<DirectionalLight>(value);
      //     program->set_uniform((light_name + ".direction").c_str(),
      //                          GLUniform{light.direction});
      //     program->set_uniform((light_name + ".color").c_str(),
      //                          GLUniform{light.color});
      //     program->set_uniform((light_name + ".intensity").c_str(),
      //                          GLUniform{light.intensity});
      //   } catch (const std::bad_variant_access &) {
      //     // PointLight light = std::get<PointLight>(value);
      //     // program->set_uniform((light_name + ".position").c_str(),
      //     //                      GLUniform{light.position});
      //     // program->set_uniform((light_name + ".color").c_str(),
      //     //                      GLUniform{light.color});
      //     // program->set_uniform((light_name + ".intensity").c_str(),
      //     //                      GLUniform{light.intensity});
      //     spdlog::error("Unsupported light type");
      //     throw std::runtime_error("Unsupported light type");
      //   }
    }
    // program->set_uniform("light.direction",
    //                      GLUniform{lights[0].direction});
    // program->set_uniform("light.color", GLUniform{lights[0].color});
    // program->set_uniform("light.intensity",
    //                      GLUniform{lights[0].intensity});
    program->set_uniform("camera_pos",
                         GLUniform{camera->transform().position()});
    program->set_uniform("model", GLUniform{glm::mat4(1.0f)});
    program->set_uniform("view", GLUniform{camera->view_matrix()});
    program->set_uniform("projection", GLUniform{camera->projection_matrix()});
    glBindVertexArray(geometry->handle());
    if (geometry->has_indices()) {
      glDrawElements(GL_TRIANGLES, geometry->count(), GL_UNSIGNED_SHORT, 0);
    } else {
      glDrawArrays(GL_TRIANGLES, 0, geometry->count());
    }
  }
}