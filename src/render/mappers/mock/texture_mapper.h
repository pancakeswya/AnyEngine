#ifndef RENDER_MAPPERS_MOCK_TEXTURE_MAPPER_H_
#define RENDER_MAPPERS_MOCK_TEXTURE_MAPPER_H_

#include "render/texture_transferer.h"

#include <vector>

namespace mock {

class TextureMapper final : public render::TextureMapper {
public:
  static constexpr size_t kWidth = 16;
  static constexpr size_t kHeight = 16;

  TextureMapper() : pixels_(kWidth * kHeight, 0xff) {}

  ~TextureMapper() override = default;

  [[nodiscard]] render::TextureInfo info() const noexcept override {
    return {
      .width = kWidth,
      .height = kHeight,
      .format = SDL_PIXELFORMAT_RGBA32
    };
  }

  uint8_t* Map(const SDL_PixelFormat format) override {
    return pixels_.data();
  }
private:
  std::vector<uint8_t> pixels_;
};

} // namespace mock

#endif // RENDER_MAPPERS_MOCK_TEXTURE_MAPPER_H_
