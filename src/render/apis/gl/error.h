#ifndef RENDER_APIS_GL_ERROR_H_
#define RENDER_APIS_GL_ERROR_H_

#include "render/error.h"
#include "render/apis/gl/loader.h"

namespace gl {

struct Error final : render::Error {
  using render::Error::Error;

  [[nodiscard]] Error WithShaderMessage(GLuint who) const;
};

} // namespace gl

#endif // RENDER_APIS_GL_ERROR_H_
