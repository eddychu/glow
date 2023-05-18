#include <scene/material.h>

uint32_t g_material_id = 0;

ShaderSource default_shader_source = {"assets/shaders/basic.vert.glsl",
                                      "assets/shaders/basic.frag.glsl"};