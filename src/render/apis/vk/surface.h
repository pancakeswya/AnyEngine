#ifndef RENDER_APIS_VK_SURFACE_H_
#define RENDER_APIS_VK_SURFACE_H_

#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>

namespace vk {

struct Surface : NonDispatchableHandle<VkSurfaceKHR, VkInstance, vkDestroySurfaceKHR> {
  DECLARE_DEFAULT_NO_COPY_CLASS(Surface);
  Surface(SDL_Window* window, VkInstance instance);
};

} // namespace vk

#endif // RENDER_APIS_VK_SURFACE_H_
