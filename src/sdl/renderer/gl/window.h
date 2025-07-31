#ifndef SDL_RENDERER_GL_WINDOW_H_
#define SDL_RENDERER_GL_WINDOW_H_

#include "base/decl.h"
#include "render/apis/gl/window.h"

#include <SDL3/SDL_video.h>

namespace sdl::gl {

class Window final : public ::gl::Window {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Window);

  explicit Window(SDL_Window* window);

  void GetSize(int& width, int& height) const noexcept override;

  [[nodiscard]] operator SDL_Window*() const {
    return window_;
  }
private:
  SDL_Window* window_ = nullptr;
};

} // namespace sdl::gl

#endif // SDL_RENDERER_GL_WINDOW_H_
