#ifndef RENDER_APIS_GL_PROGRAM_H_
#define RENDER_APIS_GL_PROGRAM_H_

#include "render/apis/gl/loader.h"
#include "render/apis/gl/handle.h"

#include <vector>

namespace gl {

struct ShaderInfo {
  const char* code;
  GLenum type;
};

struct Shader : Handle<GLuint, glDeleteShader> {
  DECLARE_DEFAULT_NO_COPY_CLASS(Shader);

  explicit Shader(const ShaderInfo& info);
};

struct ShaderProgram : Handle<GLuint, glDeleteProgram> {
  DECLARE_DEFAULT_NO_COPY_CLASS(ShaderProgram);

  explicit ShaderProgram(const std::vector<ShaderInfo>& infos);
};

} // namespace gl

#endif // RENDER_APIS_GL_PROGRAM_H_
