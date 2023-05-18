#include <utils/calc_tangent.h>
#include <scene/geometry.h>
#include <spdlog/spdlog.h>
CalcTangents::CalcTangents() {
  iface.m_getNumFaces = get_num_faces;
  iface.m_getNumVerticesOfFace = get_num_vertices_of_face;

  iface.m_getNormal = get_normal;
  iface.m_getPosition = get_position;
  iface.m_getTexCoord = get_tex_coords;
  iface.m_setTSpace = set_tspace;

  context.m_pInterface = &iface;
}

void CalcTangents::calc(Geometry *mesh) {

  context.m_pUserData = mesh;

  genTangSpaceDefault(&this->context);
}

int CalcTangents::get_num_faces(const SMikkTSpaceContext *context) {
  Geometry *working_mesh = static_cast<Geometry *>(context->m_pUserData);

  float f_size = (float)working_mesh->indices.size() / 3.f;
  int i_size = (int)working_mesh->indices.size() / 3;

  assert((f_size - (float)i_size) == 0.f);

  return i_size;
}

int CalcTangents::get_num_vertices_of_face(const SMikkTSpaceContext *context,
                                           const int iFace) {
  Geometry *working_mesh = static_cast<Geometry *>(context->m_pUserData);

  if (working_mesh->mode == GeometryMode::Triangles) {
    return 3;
  }
  throw std::logic_error(
      "no vertices with less than 3 and more than 3 supported");
}

void CalcTangents::get_position(const SMikkTSpaceContext *context,
                                float *outpos, const int iFace,
                                const int iVert) {

  auto *working_mesh = static_cast<Geometry *>(context->m_pUserData);

  auto index = get_vertex_index(context, iFace, iVert);
  auto vertex = working_mesh->vertices[index];
  outpos[0] = vertex.position.x;
  outpos[1] = vertex.position.y;
  outpos[2] = vertex.position.z;
}

void CalcTangents::get_normal(const SMikkTSpaceContext *context,
                              float *outnormal, const int iFace,
                              const int iVert) {
  auto *working_mesh = static_cast<Geometry *>(context->m_pUserData);

  auto index = get_vertex_index(context, iFace, iVert);
  auto vertex = working_mesh->vertices[index];

  outnormal[0] = vertex.normal.x;
  outnormal[1] = vertex.normal.y;
  outnormal[2] = vertex.normal.z;
}

void CalcTangents::get_tex_coords(const SMikkTSpaceContext *context,
                                  float *outuv, const int iFace,
                                  const int iVert) {
  auto *working_mesh = static_cast<Geometry *>(context->m_pUserData);

  auto index = get_vertex_index(context, iFace, iVert);
  auto vertex = working_mesh->vertices[index];

  outuv[0] = vertex.uv.x;
  outuv[1] = vertex.uv.y;
}

void CalcTangents::set_tspace(const SMikkTSpaceContext *context,
                              const float fvTangent[],
                              const float fvBiTangent[], const float fMagS,
                              const float fMagT,
                              const tbool bIsOrientationPreserving,
                              const int iFace, const int iVert) {
  auto *working_mesh = static_cast<Geometry *>(context->m_pUserData);

  auto index = get_vertex_index(context, iFace, iVert);
  auto *vertex = &working_mesh->vertices[index];

  vertex->tangent.x = fvTangent[0];
  vertex->tangent.y = fvTangent[1];
  vertex->tangent.z = fvTangent[2];

  vertex->bitangent.x = fvBiTangent[0];
  vertex->bitangent.y = fvBiTangent[1];
  vertex->bitangent.z = fvBiTangent[2];
}

int CalcTangents::get_vertex_index(const SMikkTSpaceContext *context, int iFace,
                                   int iVert) {
  auto *working_mesh = static_cast<Geometry *>(context->m_pUserData);

  auto face_size = get_num_vertices_of_face(context, iFace);

  auto indices_index = (iFace * face_size) + iVert;

  int index = working_mesh->indices[indices_index];
  return index;
}