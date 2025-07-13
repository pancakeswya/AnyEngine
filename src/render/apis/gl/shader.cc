#include "render/apis/gl/shader.h"

#include "render/apis/gl/error.h"

namespace gl {

namespace {

inline void LinkShaderProgram(const GLuint program) {
  glLinkProgram(program);
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    throw Error("Failed to link shader program").WithShaderMessage(program);
  }
}

} // namespace

Shader::Shader(const ShaderInfo& info) : Handle(glCreateShader(info.type)) {
  glShaderSource(handle_, 1, &info.code, nullptr);
  glCompileShader(handle_);

  GLint status;
  glGetShaderiv(handle_, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    throw Error("Failed to compile shader").WithShaderMessage(handle_);
  }
}

ShaderProgram::ShaderProgram(const std::vector<ShaderInfo>& infos) : Handle(glCreateProgram()) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);

  std::vector<Shader> shaders;
  for(const ShaderInfo& info : infos) {
    shaders.emplace_back(info);
  }
  for(const Shader& shader : shaders) {
    glAttachShader(handle_, shader);
  }
  LinkShaderProgram(handle_);
  glUseProgram(handle_);
}

} // namespace gl