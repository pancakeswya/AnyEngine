#ifndef SDL_RENDERER_GL_IMPL_H_
#define SDL_RENDERER_GL_IMPL_H_

#include "sdl/renderer/gl/gui.h"
#include "sdl/renderer/gl/window.h"
#include "sdl/renderer/renderer.h"

#include "render/apis/gl/api.h"

#include <SDL3/SDL_video.h>

#include <memory>

namespace sdl::gl {

class RendererImpl final : public Renderer {
public:
  RendererImpl(SDL_Window* window, float scale_factor);
  ~RendererImpl() override = default;

  void RenderFrame() override;
  render::Object* LoadObject(
    render::GeometryTransferer& geometry_transferer,
    const std::vector<std::string>& texture_paths
  ) override;
  void OnResize(int width, int height) override;
private:
  struct GLContextDeleter {
    void operator()(SDL_GLContext context) const noexcept { SDL_GL_DestroyContext(context); }
  };

  Window window_;
  std::unique_ptr<SDL_GLContextState, GLContextDeleter> context_;
  ::gl::Api renderer_;
  GuiRenderer gui_renderer_;
};

} // sdl::gl

#endif // SDL_RENDERER_GL_IMPL_H_
