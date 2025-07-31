#ifndef RENDER_APIS_VK_SURFACE_H_
#define RENDER_APIS_VK_SURFACE_H_

#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

namespace vk {

using Surface = NonDispatchableHandle<VkSurfaceKHR, VkInstance, vkDestroySurfaceKHR>;

} // namespace vk

#endif // RENDER_APIS_VK_SURFACE_H_
