#include "sdl/gui/gui.h"

#include <backends/imgui_impl_sdl3.h>

namespace sdl {

GuiRenderer::GuiRenderer(const float scale_factor) : Renderer(scale_factor) {}

GuiRenderer::~GuiRenderer() {
  ImGui_ImplSDL3_Shutdown();
}

void GuiRenderer::RenderFrame() {
  ImGui_ImplSDL3_NewFrame();
  Renderer::RenderFrame();
}

void GuiRenderer::ProcessEvent(const SDL_Event* event) {
  ImGui_ImplSDL3_ProcessEvent(event);
}

} // namespace sdl