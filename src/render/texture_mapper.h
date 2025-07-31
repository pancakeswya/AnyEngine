#ifndef RENDER_TEXTURE_MAPPER_H_
#define RENDER_TEXTURE_MAPPER_H_

#include "render/texture.h"

#include <cstdint>

namespace render {

struct TextureMapper {
  virtual ~TextureMapper() = default;
  [[nodiscard]] virtual TextureInfo info() const noexcept = 0;
  virtual uint8_t* Map() = 0;
};

} // namespace render

#endif // RENDER_TEXTURE_MAPPER_H_
