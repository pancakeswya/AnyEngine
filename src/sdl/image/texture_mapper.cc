#include <SDL3_image/SDL_image.h>

#include "sdl/image/error.h"
#include "sdl/image/texture_mapper.h"

namespace sdl {

namespace {

SDL_Surface* LoadSurface(const std::string& filepath) {
  SDL_Surface* surface = IMG_Load(filepath.c_str());
  if (surface == nullptr) {
    throw Error("Failed to load image: " + filepath);
  }
  return surface;
}

} // namespace

TextureMapper::TextureMapper(const std::string& filepath)
  : surface_(LoadSurface(filepath)) {}

TextureMapper::~TextureMapper() {
  SDL_DestroySurface(surface_);
}

render::TextureInfo TextureMapper::info() const noexcept {
  return {
    .width = static_cast<uint32_t>(surface_->w),
    .height = static_cast<uint32_t>(surface_->h),
    .bytes_per_pixel = static_cast<uint32_t>(SDL_BYTESPERPIXEL(surface_->format)),
  };
}

uint8_t* TextureMapper::Map() {
  if (!SDL_FlipSurface(surface_, SDL_FLIP_VERTICAL)) {
    throw Error(std::string("Failed to flip surface: ") + SDL_GetError());
  }
  return static_cast<uint8_t*>(surface_->pixels);
}

void TextureMapper::Reformat(const SDL_PixelFormat format) {
  if (surface_->format != format) {
    SDL_Surface* surface = SDL_ConvertSurface(surface_, format);
    if (surface == nullptr) {
      throw Error(std::string("Failed to convert surface: ") + SDL_GetError());
    }
    SDL_DestroySurface(surface_);
    surface_ = surface;
  }
}

} // namespace sdl