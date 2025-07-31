#include "sdl/renderer/gl/texture_mapper.h"

namespace sdl::gl {

namespace {

GLenum MapFormat(const SDL_PixelFormat format) noexcept {
  switch (format) {
    case SDL_PIXELFORMAT_RGBA32:
      return GL_RGBA;
    default:
      break;
  }
  return GL_INVALID_ENUM;
}

} // namespace

GLenum TextureMapperMock::format() const noexcept {
  return MapFormat(sdl::TextureMapperMock::kPixelFormat);
}

TextureMapper::TextureMapper(const std::string& filepath)
  : sdl::TextureMapper(filepath),
    format_(Remap(MapFormat, static_cast<GLenum>(GL_INVALID_ENUM))) {}

} // namespace sdl::gl