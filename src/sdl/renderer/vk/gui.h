#ifndef SDL_RENDERER_VK_GUI_H_
#define SDL_RENDERER_VK_GUI_H_

#include "sdl/gui/gui.h"

#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>
#include <SDL3/SDL_video.h>

namespace sdl::vk {

struct GuiRenderer final : sdl::GuiRenderer {
  static void Record(VkCommandBuffer command_buffer);

  GuiRenderer(SDL_Window* window, float scale_factor, ImGui_ImplVulkan_InitInfo* info);
  ~GuiRenderer() override;
  void RenderFrame() override;
};

} // namespace sdl::vk

#endif // SDL_RENDERER_VK_GUI_H_
