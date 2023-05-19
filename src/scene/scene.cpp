#include <scene/geometry.h>
#include <spdlog/spdlog.h>
#include <utils/calc_tangent.h>
#include <scene/scene.h>
#include <stdint.h>
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

static std::string current_direrctory = "";

void process_mesh(Scene &scene, uint32_t index, const tinygltf::Mesh &mesh,
                  const tinygltf::Model &model) {
  auto &m = scene.meshes[index];
  m.sub_meshes.resize(mesh.primitives.size());
  glm::vec3 mesh_min = glm::vec3(FLT_MAX);
  glm::vec3 mesh_max = glm::vec3(-FLT_MAX);
  for (uint32_t i = 0; i < mesh.primitives.size(); i++) {
    glm::vec3 sub_mesh_min = glm::vec3(FLT_MAX);
    glm::vec3 sub_mesh_max = glm::vec3(-FLT_MAX);
    auto &primitive = mesh.primitives[i];
    auto &sub_mesh = m.sub_meshes[i];

    const float *buffer_pos = nullptr;
    const float *buffer_norm = nullptr;
    const float *buffer_uv = nullptr;
    // const float* buffer_uv1 = nullptr;

    uint32_t pos_byte_stride = 0;
    uint32_t norm_byte_stride = 0;
    uint32_t uv_byte_stride = 0;

    const tinygltf::Accessor &pos_accessor =
        model.accessors[primitive.attributes.find("POSITION")->second];
    const tinygltf::BufferView &pos_view =
        model.bufferViews[pos_accessor.bufferView];
    buffer_pos = reinterpret_cast<const float *>(
        &(model.buffers[pos_view.buffer]
              .data[pos_accessor.byteOffset + pos_view.byteOffset]));
    uint32_t vertex_count = static_cast<uint32_t>(pos_accessor.count);
    pos_byte_stride = static_cast<uint32_t>(
        pos_accessor.ByteStride(pos_view)
            ? (pos_accessor.ByteStride(pos_view) / sizeof(float))
            : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3));

    if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
      const tinygltf::Accessor &norm_accessor =
          model.accessors[primitive.attributes.find("NORMAL")->second];
      const tinygltf::BufferView &norm_view =
          model.bufferViews[norm_accessor.bufferView];
      buffer_norm = reinterpret_cast<const float *>(
          &(model.buffers[norm_view.buffer]
                .data[norm_accessor.byteOffset + norm_view.byteOffset]));

      norm_byte_stride = static_cast<uint32_t>(
          norm_accessor.ByteStride(norm_view)
              ? (norm_accessor.ByteStride(norm_view) / sizeof(float))
              : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3));
    }

    if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
      const tinygltf::Accessor &uv_accessor =
          model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
      const tinygltf::BufferView &uv_view =
          model.bufferViews[uv_accessor.bufferView];
      buffer_uv = reinterpret_cast<const float *>(
          &(model.buffers[uv_view.buffer]
                .data[uv_accessor.byteOffset + uv_view.byteOffset]));

      uv_byte_stride = static_cast<uint32_t>(
          uv_accessor.ByteStride(uv_view)
              ? (uv_accessor.ByteStride(uv_view) / sizeof(float))
              : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC2));
    }

    // if (primitive.attributes.find("TEXCOORD_1") !=
    // primitive.attributes.end()) {
    //   const tinygltf::Accessor &uv_accessor =
    //       model.accessors[primitive.attributes.find("TEXCOORD_1")->second];
    //   const tinygltf::BufferView &uv_view =
    //       model.bufferViews[uv_accessor.bufferView];
    //   const float *buffer_uv1 = reinterpret_cast<const float *>(
    //       &(model.buffers[uv_view.buffer]
    //             .data[uv_accessor.byteOffset + uv_view.byteOffset]));
    //   uint32_t uv1_byte_offset =
    //       uv_accessor.ByteStride(uv_view)
    //           ? (uv_accessor.ByteStride(uv_view) / sizeof(float))
    //           : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC2);
    // }

    // if (primitive.attributes.find("COLOR_0") != primitive.attributes.end()) {
    //   const tinygltf::Accessor &color_accessor =
    //       model.accessors[primitive.attributes.find("COLOR_0")->second];
    //   const tinygltf::BufferView &color_view =
    //       model.bufferViews[color_accessor.bufferView];
    //   const float *buffer_color = reinterpret_cast<const float *>(
    //       &(model.buffers[color_view.buffer]
    //             .data[color_accessor.byteOffset + color_view.byteOffset]));
    //   uint32_t color_byte_stride = static_cast<uint32_t>(
    //       color_accessor.ByteStride(color_view)
    //           ? (color_accessor.ByteStride(color_view) / sizeof(float))
    //           : tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3));
    // }
    std::vector<Vertex> vertices(vertex_count);
    // TODO: handle joints and skinning.
    for (uint32_t i = 0; i < pos_accessor.count; i++) {
      auto &v = vertices[i];
      v.position = glm::make_vec3(&buffer_pos[i * pos_byte_stride]);
      sub_mesh_min = glm::min(sub_mesh_min, v.position);
      sub_mesh_max = glm::max(sub_mesh_max, v.position);
      if (buffer_norm) {
        v.normal = glm::make_vec3(&buffer_norm[i * norm_byte_stride]);
      }
      if (buffer_uv) {
        v.uv = glm::make_vec2(&buffer_uv[i * uv_byte_stride]);
      }
    }
    std::vector<uint32_t> indices;
    if (primitive.indices > -1) {
      const tinygltf::Accessor &accessor = model.accessors[primitive.indices];
      const tinygltf::BufferView &buffer_view =
          model.bufferViews[accessor.bufferView];
      const tinygltf::Buffer &buffer = model.buffers[buffer_view.buffer];

      indices.resize(accessor.count);

      const void *data_ptr =
          &(buffer.data[accessor.byteOffset + buffer_view.byteOffset]);
      switch (accessor.componentType) {
      case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
        const uint32_t *buf = static_cast<const uint32_t *>(data_ptr);
        for (size_t index = 0; index < accessor.count; index++) {
          indices[index] = buf[index];
        }
        break;
      }
      case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
        const uint16_t *buf = static_cast<const uint16_t *>(data_ptr);
        for (size_t index = 0; index < accessor.count; index++) {
          indices[index] = buf[index];
        }
        break;
      }
      case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
        const uint8_t *buf = static_cast<const uint8_t *>(data_ptr);
        for (size_t index = 0; index < accessor.count; index++) {
          indices[index] = buf[index];
        }
        break;
      }
      }
    }

    sub_mesh.geometry.vertices = vertices;
    sub_mesh.geometry.indices = indices;
    sub_mesh.bbox.min = sub_mesh_min;
    sub_mesh.bbox.max = sub_mesh_max;


    CalcTangents calc;
    calc.calc(&sub_mesh.geometry);

    if (primitive.material > -1) {
      sub_mesh.material = primitive.material;
    }

    mesh_min = glm::min(mesh_min, sub_mesh_min);
    mesh_max = glm::max(mesh_max, sub_mesh_max);
  }

  m.bbox.min = mesh_min;
  m.bbox.max = mesh_max;
}

void process_material(Scene &scene, uint32_t index,
                      const tinygltf::Material &material,
                      const tinygltf::Model &model) {
  auto &mat = scene.materials[index];

  // FIX: RIGHT NOW THE ORDER of the textures is important.

  if (material.pbrMetallicRoughness.baseColorTexture.index > -1) {
    auto texture_index = material.pbrMetallicRoughness.baseColorTexture.index;
    mat.add_texture(texture_index);
  }
  if (material.normalTexture.index > -1) {
    auto texture_index = material.normalTexture.index;
    mat.add_texture(texture_index);
  }

  if (material.pbrMetallicRoughness.metallicRoughnessTexture.index > -1) {
    auto texture_index =
        material.pbrMetallicRoughness.metallicRoughnessTexture.index;
    mat.add_texture(texture_index);
  }

  if (material.occlusionTexture.index > -1) {
    auto texture_index = material.occlusionTexture.index;
    mat.add_texture(texture_index);
  }

  if (material.emissiveTexture.index > -1) {
    auto texture_index = material.emissiveTexture.index;
    mat.add_texture(texture_index);
  }
}

void process_node(Scene &scene, uint32_t index, const tinygltf::Node &node,
                  const tinygltf::Model &model) {
  auto &n = scene.nodes[index];
  if (node.translation.size() == 3) {
    n.transform.set_position(glm::make_vec3(node.translation.data()));
  }

  if (node.rotation.size() == 4) {
    n.transform.set_rotation(glm::make_quat(node.rotation.data()));
  }

  if (node.scale.size() == 3) {
    n.transform.set_scale(glm::make_vec3(node.scale.data()));
  }

  if (node.mesh > -1) {
    n.mesh = node.mesh;
    n.bbox = scene.meshes[node.mesh].bbox;
  }
  if (node.children.size() > 0) {
    n.first_child = node.children.front();
    n.num_children = node.children.size();
    for (uint32_t i = 0; i < node.children.size(); i++) {
      scene.nodes[n.first_child + i].parent = index;
      process_node(scene, n.first_child + i, model.nodes[node.children[i]],
                   model);
    }
    for (uint32_t i = 0; i < node.children.size(); i++) {
      n.bbox.expand(scene.nodes[n.first_child + i].bbox);
    }
  }

}

Scene load_scene(const std::string &filename) {
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;

  std::string err;
  std::string warn;

  std::string ext = filename.substr(filename.find_last_of(".") + 1);

  bool ret = false;

  if (ext.compare("glb") == 0) {
    ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename.c_str());
  } else {
    ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename.c_str());
  }

  if (!warn.empty()) {
    spdlog::warn("{}", warn);
  }

  if (!err.empty()) {
    spdlog::error("{}", err);

    throw std::runtime_error("Failed to load scene");
  }

  const auto &scene =
      model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];
  Scene result{};
  result.meshes.resize(model.meshes.size());
  result.nodes.resize(scene.nodes.size());
  result.materials.resize(model.materials.size());

  for (uint32_t i = 0; i < scene.nodes.size(); i++) {
    const auto &node = model.nodes[scene.nodes[i]];
    process_node(result, i, node, model);
  }
  result.textures.resize(model.textures.size());
  for (uint32_t i = 0; i < model.textures.size(); i++) {
    const auto &texture = model.textures[i];
    const auto &image = model.images[texture.source];
    const auto &sampler = model.samplers[texture.sampler];

    // std::vector<unsigned char> image_data(image.image.size() *
    // image.component); memcpy(image_data.data(), image.image.data(),
    // image.image.size());

    result.textures[i].width = image.width;
    result.textures[i].height = image.height;
    result.textures[i].component = image.component;
    result.textures[i].data = image.image;
    result.textures[i].sampler.min_filter = Sampler::filter_from_int(sampler.minFilter);
    result.textures[i].sampler.mag_filter = Sampler::filter_from_int(sampler.magFilter);
    result.textures[i].sampler.wrap_s = Sampler::wrap_from_int(sampler.wrapS);
    result.textures[i].sampler.wrap_t = Sampler::wrap_from_int(sampler.wrapT);
  }

  for (uint32_t i = 0; i < result.materials.size(); i++) {
    const auto &material = model.materials[i];
    process_material(result, i, material, model);
  }

  for (uint32_t i = 0; i < result.meshes.size(); i++) {
    const auto &mesh = model.meshes[i];
    process_mesh(result, i, mesh, model);
  }

  result.bbox = result.nodes[0].bbox;

  return result;
}

