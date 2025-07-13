#include <SDL3_image/SDL_image.h>

#include <cstring>

#include "render/mappers/sdl/error.h"
#include "render/mappers/sdl/texture_mapper.h"

namespace sdl {

TextureMapper::TextureMapper(const std::string& path) : surface_(IMG_Load(path.c_str())) {
  if (surface_ == nullptr) {
    throw Error("Failed to load image: " + path);
  }
}

TextureMapper::~TextureMapper() {
  SDL_DestroySurface(surface_);
}

render::TextureInfo TextureMapper::info() const noexcept {
  return {
    .width = static_cast<uint32_t>(surface_->w),
    .height = static_cast<uint32_t>(surface_->h),
    .format = surface_->format
  };
}

uint8_t* TextureMapper::Map(const SDL_PixelFormat format) {
  if (surface_->format != format) {
    SDL_Surface* surface = SDL_ConvertSurface(surface_, format);
    if (surface == nullptr) {
      throw Error(std::string("Failed to convert surface: ") + SDL_GetError());
    }
    SDL_DestroySurface(surface_);
    surface_ = surface;
  }
  if (!SDL_FlipSurface(surface_, SDL_FLIP_VERTICAL)) {
    throw Error(std::string("Failed to flip surface: ") + SDL_GetError());
  }
  return static_cast<uint8_t*>(surface_->pixels);
}

} // namespace sdl