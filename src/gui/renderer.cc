#include "gui/renderer.h"

#include <imgui.h>

#include <sstream>

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

void Renderer::RenderFrame() {
  ImGui::NewFrame();

  ImGui::Begin("Gui");

  ImGui::Text("Fps %.2f", fps_counter_.Count());
  ImGui::End();

  ImGui::Render();
}

} // namespace gui