#ifndef SDL_IMAGE_TEXTURE_MAPPER_MOCK_H_
#define SDL_IMAGE_TEXTURE_MAPPER_MOCK_H_

#include "render/texture_mapper.h"

#include <vector>

#include <SDL3/SDL_pixels.h>

namespace sdl {

class TextureMapperMock : public virtual render::TextureMapper {
public:
  static constexpr SDL_PixelFormat kPixelFormat = SDL_PIXELFORMAT_RGBA32;

  static constexpr render::TextureInfo kTextureInfo = {
    .width = 16,
    .height = 16,
    .bytes_per_pixel = SDL_BYTESPERPIXEL(kPixelFormat)
  };

  [[nodiscard]] render::TextureInfo info() const noexcept override {
    return kTextureInfo;
  }

  uint8_t* Map() override {
    pixels_.resize(kTextureInfo.width * kTextureInfo.height * kTextureInfo.bytes_per_pixel, 0xff);
    return pixels_.data();
  }
private:
  std::vector<uint8_t> pixels_;
};

} // namespace sdl

#endif // SDL_IMAGE_TEXTURE_MAPPER_MOCK_H_
