#include "sdl/renderer/gl/gui.h"

#include "sdl/renderer/gl/error.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

namespace sdl::gl {

GuiRenderer::GuiRenderer(
  SDL_Window* window,
  SDL_GLContext context,
  const char* glsl_version,
  const float scale_factor
) : sdl::GuiRenderer(scale_factor) {
  if (!ImGui_ImplSDL3_InitForOpenGL(window, context)) {
    throw Error("Failed to initialize imgui SDL for GL");
  }
  if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
    throw Error("Failed to initialize imgui OpenGL backend");
  }
#ifdef __EMSCRIPTEN__
  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = nullptr;
#endif
}

GuiRenderer::~GuiRenderer() {
  ImGui_ImplOpenGL3_Shutdown();
}

void GuiRenderer::RenderFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  sdl::GuiRenderer::RenderFrame();
}

void GuiRenderer::Draw() {
  if (ImDrawData* draw_data = ImGui::GetDrawData(); draw_data != nullptr) {
    ImGui_ImplOpenGL3_RenderDrawData(draw_data);
  }
}

} // namespace sdl::gl
