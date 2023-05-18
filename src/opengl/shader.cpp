#include "scene/material.h"
#include <opengl/resource.h>
#include <fstream>
#include <opengl/shader.h>
#include <istream>
#include <string>
#include <variant>
#include <spdlog/spdlog.h>
std::string read_file_to_string(const char *filename) {
  std::ifstream file(filename);
  std::string str;
  std::string file_contents;
  while (std::getline(file, str)) {
    file_contents += str;
    file_contents.push_back('\n');
  }
  return file_contents;
}
GLShader::GLShader(const char *path, GLenum type) {
  handle = glCreateShader(type);
  std::string content = read_file_to_string(path);
  const char *c_str = content.c_str();
  glShaderSource(handle, 1, &c_str, NULL);
  glCompileShader(handle);
  GLint success;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(handle, 512, NULL, infoLog);
    spdlog::error("Error: Failed to compile shader {}.", path);
    spdlog::error("{}", infoLog);
    throw std::runtime_error("Failed to compile shader.");
  }
}

void GLShader::destroy() const { glDeleteShader(handle); }

GLProgram::GLProgram(const Material &material) {
  set_id(material.id());
  const char *vs_path = material.shader_source().vertex_path.c_str();
  const char *fs_path = material.shader_source().fragment_path.c_str();
  GLShader vs(vs_path, GL_VERTEX_SHADER);
  GLShader fs(fs_path, GL_FRAGMENT_SHADER);
  handle = glCreateProgram();
  glAttachShader(handle, vs.handle);
  glAttachShader(handle, fs.handle);
  glLinkProgram(handle);
  GLint success;
  glGetProgramiv(handle, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(handle, 512, NULL, infoLog);
    spdlog::error("Error: Failed to link program.");
    spdlog::error("{}", infoLog);
    throw std::runtime_error("Failed to link program.");
  }
  glDetachShader(handle, vs.handle);
  glDetachShader(handle, fs.handle);
  vs.destroy();
  fs.destroy();
  populate_uniforms();
}

void GLProgram::populate_uniforms() {
  glUseProgram(handle);
  GLint numUniforms = 0;
  glGetProgramInterfaceiv(handle, GL_UNIFORM, GL_ACTIVE_RESOURCES,
                          &numUniforms);

  GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};

  for (GLint i = 0; i < numUniforms; ++i) {
    GLint results[4];
    glGetProgramResourceiv(handle, GL_UNIFORM, i, 4, properties, 4, NULL,
                           results);

    if (results[3] != -1)
      continue; // Skip uniforms in blocks
    GLint nameBufSize = results[0] + 1;
    char *name = new char[nameBufSize];
    glGetProgramResourceName(handle, GL_UNIFORM, i, nameBufSize, NULL, name);
    uniforms[name] = results[2];
    delete[] name;
  }

  // log all uniforms
  for (auto &uniform : uniforms) {
    printf("Uniform %s at location %d\n", uniform.first.c_str(),
           uniform.second);
  }

  glUseProgram(0);
}

void GLProgram::use() const { glUseProgram(handle); }

void GLProgram::destroy() const { glDeleteProgram(handle); }

void GLProgram::set_uniform(const char *name,
                            const UniformValue &uniform) const {

  GLint location = glGetUniformLocation(handle, name);
  if (location == -1) {
    printf("Error: Uniform %s not found.\n", name);
    return;
  }
  if (std::holds_alternative<mat4>(uniform)) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &std::get<mat4>(uniform)[0][0]);
  } else if (std::holds_alternative<mat3>(uniform)) {
    glUniformMatrix3fv(location, 1, GL_FALSE, &std::get<mat3>(uniform)[0][0]);
  } else if (std::holds_alternative<vec4>(uniform)) {
    glUniform4fv(location, 1, &std::get<vec4>(uniform)[0]);
  } else if (std::holds_alternative<vec3>(uniform)) {
    glUniform3fv(location, 1, &std::get<vec3>(uniform)[0]);
  } else if (std::holds_alternative<vec2>(uniform)) {
    glUniform2fv(location, 1, &std::get<vec2>(uniform)[0]);
  } else if (std::holds_alternative<float>(uniform)) {
    glUniform1f(location, std::get<float>(uniform));
  } else if (std::holds_alternative<int>(uniform)) {
    glUniform1i(location, std::get<int>(uniform));
  } else {
    printf("Error: Uniform type not supported.\n");
  }
}

void GLProgram::unuse() const { glUseProgram(0); }