#ifndef SDL_RENDERER_GL_GUI_H_
#define SDL_RENDERER_GL_GUI_H_

#include "sdl/gui/gui.h"

#include "render/apis/gl/loader.h"
#include <SDL3/SDL_video.h>

namespace sdl::gl {

struct GuiRenderer final : sdl::GuiRenderer {
  GuiRenderer(
    SDL_Window* window,
    SDL_GLContext context,
    const char* glsl_version,
    float scale_factor
  );

  ~GuiRenderer() override;

  void RenderFrame() override;
  static void Draw();
};

} // namespace sdl::gl

#endif // SDL_RENDERER_GL_GUI_H_
