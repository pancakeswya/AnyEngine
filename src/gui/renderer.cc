#include "gui/renderer.h"

#include <imgui.h>

namespace gui {

Renderer::Renderer(const float scale_factor) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(scale_factor);
  style.FontScaleDpi = scale_factor;
}

Renderer::~Renderer() {
  ImGui::DestroyContext();
}

void Renderer::RenderFrame() const {
  ImGui::NewFrame();

  ImGui::Begin("Hello, world!");

  ImGui::Text("This is some useful text.");
  ImGui::End();

  ImGui::Render();
}


} // namespace gui