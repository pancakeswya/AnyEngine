#ifndef RENDER_APIS_GL_PROGRAM_H_
#define RENDER_APIS_GL_PROGRAM_H_

#include "render/apis/gl/loader.h"
#include "render/apis/gl/handle.h"

#include <vector>
#include <string>

namespace gl {

struct ShaderInfo {
  std::string code;
  GLenum type;
};

struct Shader : Handle {
  DECLARE_DEFAULT_NO_COPY_CLASS(Shader);

  explicit Shader(const ShaderInfo& info);
};

struct ShaderProgram : Handle {
  DECLARE_DEFAULT_NO_COPY_CLASS(ShaderProgram);

  explicit ShaderProgram(const char* glsl_version, std::vector<ShaderInfo>&& infos);
};

} // namespace gl

#endif // RENDER_APIS_GL_PROGRAM_H_
