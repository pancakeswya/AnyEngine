#include "render/apis/gl/gui.h"

#include "render/apis/gl/error.h"

namespace gl {

GuiRenderer::GuiRenderer(
  SDL_Window* window,
  SDL_GLContext context,
  const char* glsl_version,
  const float scale_factor
) : render::GuiRenderer(scale_factor) {
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
  render::GuiRenderer::RenderFrame();
}

void GuiRenderer::Draw() {
  if (ImDrawData* draw_data = ImGui::GetDrawData(); draw_data != nullptr) {
    ImGui_ImplOpenGL3_RenderDrawData(draw_data);
  }
}

void GuiRenderer::ProcessEvent(const SDL_Event* event) const {
  render::GuiRenderer::ProcessEvent(event);
}

} // namespace gl