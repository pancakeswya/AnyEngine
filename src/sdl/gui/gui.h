#ifndef SDL_GUI_GUI_H_
#define SDL_GUI_GUI_H_

#include "gui/renderer.h"

#include <SDL3/SDL.h>

namespace sdl {

struct GuiRenderer : gui::Renderer {
  static void ProcessEvent(const SDL_Event* event);

  explicit GuiRenderer(float scale_factor);
  ~GuiRenderer() override;
  void RenderFrame() override;
};

} // namespace render

#endif // SDL_GUI_GUI_H_
