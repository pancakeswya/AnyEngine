#ifndef RENDER_MAPPERS_SDL_TEXTURE_MAPPER_H_
#define RENDER_MAPPERS_SDL_TEXTURE_MAPPER_H_

#include "render/texture_transferer.h"

#include <SDL3/SDL_surface.h>

#include <string>

namespace sdl {

class TextureMapper final : public render::TextureMapper {
public:
  explicit TextureMapper(const std::string& path);
  ~TextureMapper() override;
  [[nodiscard]] render::TextureInfo info() const noexcept override;
  uint8_t* Map(SDL_PixelFormat format) override;
private:
  SDL_Surface* surface_;
};

} // namespace sdl

#endif // RENDER_MAPPERS_SDL_TEXTURE_MAPPER_H_
