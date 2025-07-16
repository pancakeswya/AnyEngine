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

Shader::Shader(const ShaderInfo& info) : Handle(glCreateShader(info.type), glDeleteShader) {
  const char* code = info.code.c_str();
  glShaderSource(handle_, 1, &code, nullptr);
  glCompileShader(handle_);

  GLint status;
  glGetShaderiv(handle_, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    throw Error("Failed to compile shader").WithShaderMessage(handle_);
  }
}

ShaderProgram::ShaderProgram(const char* glsl_version, std::vector<ShaderInfo>&& infos) : Handle(glCreateProgram(), glDeleteProgram) {
  glEnable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);

  std::vector<Shader> shaders;
  shaders.reserve(infos.size());
  for(ShaderInfo& info : infos) {
    info.code = glsl_version + info.code;
    shaders.emplace_back(info);
  }
  for(const Shader& shader : shaders) {
    glAttachShader(handle_, shader);
  }
  LinkShaderProgram(handle_);
  glUseProgram(handle_);
}

} // namespace gl