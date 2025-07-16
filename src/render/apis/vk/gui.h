#ifndef RENDER_APIS_VK_GUI_H_
#define RENDER_APIS_VK_GUI_H_

#include "render/gui.h"

#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>

namespace vk {

struct GuiRenderer final : render::GuiRenderer {
  GuiRenderer(SDL_Window* window, float scale_factor, ImGui_ImplVulkan_InitInfo* info);

  ~GuiRenderer() override;

  void RenderFrame() override;

  void ProcessEvent(const SDL_Event* event) const override;

  static void Record(VkCommandBuffer command_buffer);
};

} // namespace vk

#endif // RENDER_APIS_VK_GUI_H_
