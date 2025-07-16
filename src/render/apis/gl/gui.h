#ifndef RENDER_APIS_GL_GUI_H_
#define RENDER_APIS_GL_GUI_H_

#include "render/gui.h"
#include "render/apis/gl/loader.h"

#include <backends/imgui_impl_opengl3.h>
#include <SDL3/SDL.h>

namespace gl {

struct GuiRenderer final : render::GuiRenderer {
  GuiRenderer(
    SDL_Window* window,
    SDL_GLContext context,
    const char* glsl_version,
    float scale_factor
  );

  ~GuiRenderer() override;

  void RenderFrame() override;
  static void Draw();

  void ProcessEvent(const SDL_Event* event) const override;
};

} // namespace gl

#endif // RENDER_APIS_GL_GUI_H_
