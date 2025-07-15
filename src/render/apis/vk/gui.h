#ifndef RENDER_APIS_VK_GUI_H_
#define RENDER_APIS_VK_GUI_H_

#include "gui/renderer.h"

#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>

namespace vk {

struct GuiRenderer final : gui::Renderer {
  GuiRenderer(SDL_Window* window, ImGui_ImplVulkan_InitInfo* info);
  ~GuiRenderer() override;

  void RenderFrame() const override;

  static void Record(VkCommandBuffer command_buffer);
};

} // namespace vk

#endif // RENDER_APIS_VK_GUI_H_
