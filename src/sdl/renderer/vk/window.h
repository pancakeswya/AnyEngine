#ifndef SDL_RENDERER_VK_WINDOW_H_
#define SDL_RENDERER_VK_WINDOW_H_

#include "base/decl.h"
#include "render/apis/vk/window.h"

#include <SDL3/SDL_video.h>

namespace sdl::vk {

class Window final : public ::vk::Window {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Window);

  explicit Window(SDL_Window* window);

  [[nodiscard]] VkExtent2D size() const noexcept override;

  [[nodiscard]]operator SDL_Window*() const {
    return window_;
  }
private:
  SDL_Window* window_ = nullptr;
};

} // namespace sdl::vk

#endif // SDL_RENDERER_VK_WINDOW_H_
