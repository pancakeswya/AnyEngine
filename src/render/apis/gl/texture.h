#ifndef RENDER_APIS_GL_TEXTURE_H_
#define RENDER_APIS_GL_TEXTURE_H_

#include "render/apis/gl/loader.h"
#include "render/apis/gl/handle.h"

#include <cstdint>

namespace gl {

struct Texture : ArrayHandle<glGenTextures, glDeleteTextures> {
  DECLARE_DEFAULT_NO_COPY_CLASS(Texture);

  Texture(GLenum format, const uint8_t* data, int width, int height);

  void Bind() const {
    glBindTexture(GL_TEXTURE_2D, handle_);
  }

  void Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
};

} // namespace gl

#endif // RENDER_APIS_GL_TEXTURE_H_
