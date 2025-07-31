#ifndef SDL_RENDER3D_H_
#define SDL_RENDER3D_H_

#ifdef USE_VK_API
#include "sdl/renderer/vk/impl.h"
#else
#include "sdl/renderer/gl/impl.h"
#endif
#include "render/parsers/obj/object_parser.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

namespace sdl {

class Render3D final {
public:
  static constexpr SDL_WindowFlags kWindowFlags
#ifdef USE_VK_API
    = SDL_WINDOW_VULKAN;
#else
    = SDL_WINDOW_OPENGL;
#endif

  explicit Render3D(SDL_Window* window, float scale_factor);
  ~Render3D() = default;

  void RenderFrame();
  void ProcessEvent(const SDL_Event* event);
  render::Object* LoadObject(const std::string& filepath);
protected:
#ifdef USE_VK_API
  vk::RendererImpl
#else
  gl::RendererImpl
#endif
  renderer_;
  obj::Parser parser_;
};

} // namespace sdl

#endif // SDL_RENDER3D_H_
