#include <assimp/material.h>
#include <resource/material.h>
#include <resource/resource.h>
#include <resource/texture.h>
#include <memory>
#include <scene/scene.h>
#include <resource/geometry.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <stdint.h>
static std::string directory;

static void process_submesh(Mesh &mesh, aiMesh *ai_mesh,
                            const aiScene *ai_scene, ResourceCache *cache) {
  // process submesh
  SubMesh submesh;
  submesh.material_id = ai_mesh->mMaterialIndex;
  std::vector<Vertex> vertices;
  // process vertex positions, normals and texture coordinates
  for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector; // we declare a placeholder vector since assimp uses its
                      // own vector class that doesn't directly convert to glm's
                      // vec3 class so we transfer the data to this placeholder
                      // glm::vec3 first.
    // positions
    vector.x = ai_mesh->mVertices[i].x;
    vector.y = ai_mesh->mVertices[i].y;
    vector.z = ai_mesh->mVertices[i].z;
    vertex.position = vector;
    // normals
    if (ai_mesh->HasNormals()) {
      vector.x = ai_mesh->mNormals[i].x;
      vector.y = ai_mesh->mNormals[i].y;
      vector.z = ai_mesh->mNormals[i].z;
      vertex.normal = vector;
    }
    // texture coordinates
    if (ai_mesh
            ->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {
      glm::vec2 vec;
      // a vertex can contain up to 8 different texture coordinates. We thus
      // make the assumption that we won't use models where a vertex can have
      // multiple texture coordinates so we always take the first set (0).
      vec.x = ai_mesh->mTextureCoords[0][i].x;
      vec.y = ai_mesh->mTextureCoords[0][i].y;
      vertex.uv = vec;
      vector.x = ai_mesh->mTangents[i].x;
      vector.y = ai_mesh->mTangents[i].y;
      vector.z = ai_mesh->mTangents[i].z;
      vertex.tangent = vector;
      // bitangent
      vector.x = ai_mesh->mBitangents[i].x;
      vector.y = ai_mesh->mBitangents[i].y;
      vector.z = ai_mesh->mBitangents[i].z;
      vertex.bitangent = vector;
    } else {
      spdlog::error("no texture coordinates found");
      throw std::runtime_error("no texture coordinates found");
    }
    vertices.push_back(vertex);
  }

  // process indices
  std::vector<uint16_t> indices;
  for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
    aiFace face = ai_mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  auto geometry = std::make_unique<Geometry>(vertices, indices);
  submesh.geometry_id = geometry->id();

  aiMaterial *ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
  // pbr material
  // 1. albedo maps
  auto material = std::make_unique<Material>();
  submesh.material_id = material->id();

  aiString str;
  aiReturn ret = ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
  if (ret == AI_SUCCESS) {
    std::string path = directory + "/" + str.C_Str();
    auto texture = std::make_unique<Texture>(path, TextureConfig{});
    material->set_albedo_texture_id(texture->id());
    cache->add(std::move(texture));
  } else {
    spdlog::warn("no albedo texture found");
  }

  // 2. normal maps
  ret = ai_material->GetTexture(aiTextureType_NORMALS, 0, &str);
  if (ret == AI_SUCCESS) {
    std::string path = directory + "/" + str.C_Str();
    auto texture = std::make_unique<Texture>(path, TextureConfig{});
    material->set_normal_texture_id(texture->id());
    cache->add(std::move(texture));
  } else {
    spdlog::warn("no normal texture found");
  }

  // 3. metallic maps
  ret = ai_material->GetTexture(aiTextureType_METALNESS, 0, &str);
  if (ret == AI_SUCCESS) {
    std::string path = directory + "/" + str.C_Str();
    auto texture = std::make_unique<Texture>(path, TextureConfig{});
    material->set_metallic_texture_id(texture->id());
    cache->add(std::move(texture));
  } else {
    spdlog::warn("no metallic texture found");
  }

  // 4 ao
  ret = ai_material->GetTexture(aiTextureType_LIGHTMAP, 0, &str);
  if (ret == AI_SUCCESS) {
    std::string path = directory + "/" + str.C_Str();
    auto texture = std::make_unique<Texture>(path, TextureConfig{});
    material->set_ao_texture_id(texture->id());
    cache->add(std::move(texture));
  } else {
    spdlog::warn("no ao texture found {}", str.C_Str());
  }

  // 5. emissive
  ret = ai_material->GetTexture(aiTextureType_EMISSIVE, 0, &str);
  if (ret == AI_SUCCESS) {
    std::string path = directory + "/" + str.C_Str();
    auto texture = std::make_unique<Texture>(path, TextureConfig{});
    material->set_emissive_texture_id(texture->id());
    cache->add(std::move(texture));
  } else {
    spdlog::warn("no emissive texture found");
  }

  cache->add(std::move(geometry));
  cache->add(std::move(material));

  mesh.sub_meshes.push_back(submesh);
}

static void process_node(Scene &scene, aiNode *ai_node, const aiScene *ai_scene,
                         int32_t current_index, int32_t parent,
                         ResourceCache *cache) {
  // process each mesh located at the current node
  scene.nodes[current_index].parent = parent;
  if (ai_node->mNumMeshes > 0) {
    scene.nodes[current_index].mesh = scene.meshes.size();
    scene.meshes.push_back(Mesh{});
    for (unsigned int i = 0; i < ai_node->mNumMeshes; i++) {

      aiMesh *ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];

      process_submesh(scene.meshes[scene.meshes.size() - 1], ai_mesh, ai_scene,
                      cache);
    }
  }

  if (ai_node->mNumChildren > 0) {
    scene.nodes[current_index].first_child = scene.nodes.size();
    scene.nodes.resize(scene.nodes.size() + ai_node->mNumChildren);
    for (unsigned int i = 0; i < ai_node->mNumChildren; i++) {
      process_node(scene, ai_node->mChildren[i], ai_scene,
                   scene.nodes.size() - ai_node->mNumChildren + i,
                   current_index, cache);
    }
  }
}

Scene load_scene(const std::string &filename, ResourceCache *cache) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                    aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
  // check for errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) // if is Not Zero
  {
    spdlog::error("ERROR::ASSIMP:: {}", importer.GetErrorString());
    throw std::runtime_error("Failed to load scene");
  }
  // retrieve the directory path of the filepath
  directory = filename.substr(0, filename.find_last_of('/'));

  // process ASSIMP's root node recursively
  Scene result;
  result.nodes.resize(1);
  process_node(result, scene->mRootNode, scene, 0, -1, cache);
  return result;
}
