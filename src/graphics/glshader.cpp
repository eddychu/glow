#include <fstream>
#include <graphics/glshader.h>
#include <istream>
#include <string>

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
    printf("Error: Failed to compile shader %s.\n", path);
    printf("%s\n", infoLog);
  }
}

void GLShader::destroy() const { glDeleteShader(handle); }

GLProgram::GLProgram(const char *vs_path, const char *fs_path) {
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
    printf("Error: Failed to link program.\n");
    printf("%s\n", infoLog);
  }
  glDetachShader(handle, vs.handle);
  glDetachShader(handle, fs.handle);
  vs.destroy();
  fs.destroy();
}

void GLProgram::use() const { glUseProgram(handle); }

void GLProgram::destroy() const { glDeleteProgram(handle); }

void GLProgram::set_uniform(const char *name, const GLUniform &uniform) const {
  glUseProgram(handle);
  GLint location = glGetUniformLocation(handle, name);
  if (location == -1) {
    printf("Error: Uniform %s not found.\n", name);
    return;
  }
  if (std::holds_alternative<mat4>(uniform.value)) {
    glUniformMatrix4fv(location, 1, GL_FALSE,
                       &std::get<mat4>(uniform.value)[0][0]);
  } else if (std::holds_alternative<mat3>(uniform.value)) {
    glUniformMatrix3fv(location, 1, GL_FALSE,
                       &std::get<mat3>(uniform.value)[0][0]);
  } else if (std::holds_alternative<vec4>(uniform.value)) {
    glUniform4fv(location, 1, &std::get<vec4>(uniform.value)[0]);
  } else if (std::holds_alternative<vec3>(uniform.value)) {
    glUniform3fv(location, 1, &std::get<vec3>(uniform.value)[0]);
  } else if (std::holds_alternative<vec2>(uniform.value)) {
    glUniform2fv(location, 1, &std::get<vec2>(uniform.value)[0]);
  } else if (std::holds_alternative<float>(uniform.value)) {
    glUniform1f(location, std::get<float>(uniform.value));
  }
  glUseProgram(0);
}

void GLProgram::unuse() const { glUseProgram(0); }