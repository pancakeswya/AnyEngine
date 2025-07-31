#ifndef APP_H_
#define APP_H_

#include "sdl/render3d.h"
#include "sdl/error.h"
#include "render/object.h"

#include <SDL3/SDL.h>

class App {
public:
  static constexpr std::string_view kTitle = "3D Render";
  static constexpr int kWidth = 1280;
  static constexpr int kHeight = 720;

  static App* Cast(void *appstate) {
    return static_cast<App*>(appstate);
  }

  struct Error final : sdl::Error {
    using sdl::Error::Error;
  };

  App();
  ~App();

  [[nodiscard]] SDL_AppResult HandleEvent(const SDL_Event* event);
  [[nodiscard]] SDL_AppResult Iterate();
private:
  float scale_factor_ = 1.0f;
  SDL_Window* window_;
  sdl::Render3D render_;
  render::Object* object_ = nullptr;
};

#endif // APP_H_
