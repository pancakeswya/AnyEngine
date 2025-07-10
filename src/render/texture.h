#ifndef RENDER_TEXTURE_H_
#define RENDER_TEXTURE_H_

#include <SDL3/SDL_pixels.h>

#include <cstddef>

namespace render {

struct TextureIndices {
  unsigned int index;
  unsigned int offset;
};

struct TextureInfo {
  size_t width;
  size_t height;

  SDL_PixelFormat format;
};

} // namespace render

#endif // RENDER_TEXTURE_H_
