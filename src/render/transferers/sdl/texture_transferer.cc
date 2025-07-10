#include "render/transferers/sdl/texture_transferer.h"

#include <SDL3_image/SDL_image.h>

#include <cstring>

#include "render/transferers/sdl/error.h"

namespace sdl {

TextureTransferer::TextureTransferer(const std::string& path) : surface_(IMG_Load(path.c_str())) {
  if (surface_ == nullptr) {
    throw Error("Failed to load image: " + path);
  }
}

TextureTransferer::~TextureTransferer() {
  SDL_DestroySurface(surface_);
}

render::TextureInfo TextureTransferer::info() const noexcept {
  return {
    .width = static_cast<uint32_t>(surface_->w),
    .height = static_cast<uint32_t>(surface_->h),
    .format = surface_->format
  };
}

void TextureTransferer::Transfer(uint8_t* pixels, SDL_PixelFormat format) {
  if (surface_->format != format) {
    SDL_Surface* surface = SDL_ConvertSurface(surface_, format);
    if (surface == nullptr) {
      throw Error("Failed to convert surface: " + std::string(SDL_GetError()));
    }
    SDL_DestroySurface(surface_);
    surface_ = surface;
  }
  if (!SDL_FlipSurface(surface_, SDL_FLIP_VERTICAL)) {
    throw Error("Failed to flip surface: " + std::string(SDL_GetError()));
  }
  std::memcpy(pixels, surface_->pixels, surface_->pitch * surface_->h);
}

} // namespace sdl