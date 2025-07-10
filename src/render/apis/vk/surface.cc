#include "render/apis/vk/surface.h"

#include "render/apis/vk/error.h"

#include <SDL3/SDL_vulkan.h>

#include <algorithm>

namespace vk {

namespace {

VkSurfaceKHR CreateSurface(SDL_Window* window, VkInstance instance) {
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
    throw Error("failed to create window surface");
  }
  return surface;
}

} // namespace

Surface::Surface(SDL_Window* window, VkInstance instance)
  : NonDispatchableHandle(CreateSurface(window, instance), instance) {}

} // namespace vk