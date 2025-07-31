#ifndef RENDER_APIS_GL_TEXTURE_MAPPER_H_
#define RENDER_APIS_GL_TEXTURE_MAPPER_H_

#include "render/apis/gl/loader.h"
#include "render/texture_mapper.h"

namespace gl {

struct TextureMapper : virtual render::TextureMapper {
  ~TextureMapper() override = default;
  [[nodiscard]] virtual GLenum format() const noexcept = 0;
};

} // namespace gl

#endif // RENDER_APIS_GL_TEXTURE_MAPPER_H_
