#ifndef SDL_IMAGE_TEXTURE_MAPPER_H_
#define SDL_IMAGE_TEXTURE_MAPPER_H_

#include "render/texture_mapper.h"

#include <SDL3/SDL_surface.h>

#include <string>

namespace sdl {

class TextureMapper : public virtual render::TextureMapper {
public:
  explicit TextureMapper(const std::string& filepath);
  ~TextureMapper() override;
  [[nodiscard]] render::TextureInfo info() const noexcept override;
  uint8_t* Map() override;

  template<typename FormatType>
  FormatType Remap(FormatType(*map_function)(SDL_PixelFormat), FormatType invalid_value) {
    FormatType mapped_format = map_function(surface_->format);
    if (mapped_format == invalid_value) {
      Reformat(SDL_PIXELFORMAT_RGBA32);
      mapped_format = map_function(surface_->format);
    }
    return mapped_format;
  }
private:
  void Reformat(SDL_PixelFormat format);

  SDL_Surface* surface_;
};

} // namespace sdl

#endif // SDL_IMAGE_TEXTURE_MAPPER_H_
