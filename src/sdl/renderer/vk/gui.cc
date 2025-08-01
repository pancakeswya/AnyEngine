#include "sdl/renderer/vk/gui.h"

#include "sdl/renderer/vk/error.h"

#include <backends/imgui_impl_sdl3.h>

namespace sdl::vk {

GuiRenderer::GuiRenderer(
  SDL_Window* window,
  const float scale_factor,
  ImGui_ImplVulkan_InitInfo* info
) : sdl::GuiRenderer(scale_factor) {
  if (!ImGui_ImplSDL3_InitForVulkan(window)) {
    throw Error("Failed to initialize imgui SDL for vulkan");
  }
  if (!ImGui_ImplVulkan_Init(info)) {
    throw Error("Failed to initialize imgui vulkan");
  }
}

GuiRenderer::~GuiRenderer() {
  ImGui_ImplVulkan_Shutdown();
}

void GuiRenderer::Record(VkCommandBuffer command_buffer) {
  if (ImDrawData* draw_data = ImGui::GetDrawData(); draw_data != nullptr) {
    ImGui_ImplVulkan_RenderDrawData(draw_data, command_buffer);
  }
}

void GuiRenderer::RenderFrame() {
  ImGui_ImplVulkan_NewFrame();
  sdl::GuiRenderer::RenderFrame();
}

} // namespace sdl::vk