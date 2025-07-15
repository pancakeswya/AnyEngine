#ifndef RENDER_MAPPERS_MOCK_TEXTURE_MAPPER_H_
#define RENDER_MAPPERS_MOCK_TEXTURE_MAPPER_H_

#include "base/decl.h"
#include "render/texture_transferer.h"

#include <vector>

namespace mock {

class TextureMapper final : public render::TextureMapper {
public:
  static constexpr render::TextureInfo kTextureInfo = {
    .width = 16,
    .height = 16,
    .format = SDL_PIXELFORMAT_RGBA32
  };

  [[nodiscard]] render::TextureInfo info() const noexcept override {
    return kTextureInfo;
  }

  uint8_t* Map(const SDL_PixelFormat format) override {
    pixels_.resize(kTextureInfo.width * kTextureInfo.height * SDL_BYTESPERPIXEL(format), 0xff);
    return pixels_.data();
  }
private:
  std::vector<uint8_t> pixels_;
};

} // namespace mock

#endif // RENDER_MAPPERS_MOCK_TEXTURE_MAPPER_H_
