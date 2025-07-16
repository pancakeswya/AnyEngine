#ifndef RENDER_GUI_H_
#define RENDER_GUI_H_

#include "gui/renderer.h"

#include <backends/imgui_impl_sdl3.h>

namespace render {

struct GuiRenderer : gui::Renderer {
  explicit GuiRenderer(const float scale_factor) : gui::Renderer(scale_factor) {}

  ~GuiRenderer() override {
    ImGui_ImplSDL3_Shutdown();
  }

  void RenderFrame() override {
    ImGui_ImplSDL3_NewFrame();
    gui::Renderer::RenderFrame();
  }

  virtual void ProcessEvent(const SDL_Event* event) const {
    ImGui_ImplSDL3_ProcessEvent(event);
  }
};

} // namespace render

#endif // RENDER_GUI_H_
