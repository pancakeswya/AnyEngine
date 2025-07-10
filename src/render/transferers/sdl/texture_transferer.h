#ifndef RENDER_TRANSFERERS_SDL_TEXTURE_TRANSFERER_H_
#define RENDER_TRANSFERERS_SDL_TEXTURE_TRANSFERER_H_

#include "render/texture_transferer.h"

#include <SDL3/SDL_surface.h>

#include <string>

namespace sdl {

class TextureTransferer final : public render::TextureTransferer {
public:
  explicit TextureTransferer(const std::string& path);
  ~TextureTransferer() override;
  [[nodiscard]] render::TextureInfo info() const noexcept override;
  void Transfer(uint8_t* pixels, SDL_PixelFormat format) override;
private:
  SDL_Surface* surface_;
};

} // namespace sdl

#endif // RENDER_TRANSFERERS_SDL_TEXTURE_TRANSFERER_H_
