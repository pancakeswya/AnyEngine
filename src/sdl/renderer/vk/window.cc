#include "sdl/renderer/vk/window.h"

namespace sdl::vk {

Window::Window(SDL_Window* window) : window_(window) {}

VkExtent2D Window::size() const noexcept {
  int width, height;
  SDL_GetWindowSize(window_, &width, &height);
  return {
    static_cast<uint32_t>(width),
    static_cast<uint32_t>(height)
  };
}

} // namespace sdl::vk