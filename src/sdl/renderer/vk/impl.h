#ifndef SDL_RENDERER_VK_IMPL_H_
#define SDL_RENDERER_VK_IMPL_H_

#include "sdl/renderer/vk/gui.h"
#include "sdl/renderer/vk/window.h"
#include "sdl/renderer/renderer.h"
#include "render/apis/vk/instance.h"
#include "render/apis/vk/surface.h"
#include "render/apis/vk/api.h"

#include <SDL3/SDL_video.h>

namespace sdl::vk {

class RendererImpl final : public Renderer {
public:
  RendererImpl(SDL_Window* window, float scale_factor);
  ~RendererImpl() override;

  void RenderFrame() override;
  render::Object* LoadObject(
    render::GeometryTransferer& geometry_transferer,
    const std::vector<std::string>& texture_paths
  ) override;
  void OnResize(int width, int height) override;
private:
  Window window_;
  ::vk::Instance instance_;
  ::vk::Surface surface_;
  ::vk::Api renderer_;
  GuiRenderer gui_renderer_;
};

} // sdl::vk

#endif // SDL_RENDERER_VK_IMPL_H_
