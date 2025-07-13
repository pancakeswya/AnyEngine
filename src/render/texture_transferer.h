#ifndef RENDER_TEXTURE_TRANSFERER_H_
#define RENDER_TEXTURE_TRANSFERER_H_

#include "render/texture.h"

namespace render {

struct TextureMapper {
  virtual ~TextureMapper() = default;
  [[nodiscard]] virtual TextureInfo info() const noexcept = 0;
  virtual uint8_t* Map(SDL_PixelFormat format) = 0;
};

} // namespace render

#endif // RENDER_TEXTURE_TRANSFERER_H_
