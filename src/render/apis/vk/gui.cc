#include "render/apis/vk/gui.h"

#include "render/apis/vk/error.h"

#include <backends/imgui_impl_sdl3.h>

namespace vk {

GuiRenderer::GuiRenderer(SDL_Window* window, ImGui_ImplVulkan_InitInfo* info) : Renderer(SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay())) {
  if (!ImGui_ImplSDL3_InitForVulkan(window)) {
    throw Error("Failed to initialize imgui SDL for vulkan");
  }
  if (!ImGui_ImplVulkan_Init(info)) {
    throw Error("Failed to initialize imgui vulkan");
  }
}

GuiRenderer::~GuiRenderer() {
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplSDL3_Shutdown();
}

void GuiRenderer::Record(VkCommandBuffer command_buffer) {
  ImDrawData* draw_data = ImGui::GetDrawData();
  ImGui_ImplVulkan_RenderDrawData(draw_data, command_buffer);
}

void GuiRenderer::RenderFrame() const {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  gui::Renderer::RenderFrame();
}

} // namespace vk