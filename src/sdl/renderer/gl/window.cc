#include "sdl/renderer/gl/window.h"

namespace sdl::gl {

Window::Window(SDL_Window* window) : window_(window) {}

void Window::GetSize(int& width, int& height) const noexcept {
  SDL_GetWindowSize(window_, &width, &height);
}

} // namespace sdl::gl