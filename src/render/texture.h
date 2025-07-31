#ifndef RENDER_TEXTURE_H_
#define RENDER_TEXTURE_H_

#include <cstddef>

namespace render {

struct TextureIndices {
  unsigned int index;
  unsigned int offset;
};

struct TextureInfo {
  size_t width;
  size_t height;
  size_t bytes_per_pixel;
};

} // namespace render

#endif // RENDER_TEXTURE_H_
