#include "sdl/renderer/vk/texture_mapper.h"

namespace sdl::vk {

namespace {

VkFormat MapFormat(const SDL_PixelFormat format) noexcept {
  switch (format) {
    case SDL_PIXELFORMAT_RGBA32: return VK_FORMAT_R8G8B8A8_UNORM;
    default:
      break;
  }
  return VK_FORMAT_UNDEFINED;
}

} // namespace

VkFormat TextureMapperMock::format() const noexcept {
  return MapFormat(sdl::TextureMapperMock::kPixelFormat);
}

TextureMapper::TextureMapper(const std::string& filepath)
  : sdl::TextureMapper(filepath),
    format_(Remap(MapFormat, VK_FORMAT_UNDEFINED)) {}

} // namespace sdl::vk