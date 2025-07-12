#ifndef RENDER_APIS_VK_SYNC_OBJECT_H_
#define RENDER_APIS_VK_SYNC_OBJECT_H_

#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

namespace vk {

struct Semaphore : NonDispatchableHandle<VkSemaphore, VkDevice, vkDestroySemaphore> {
  DECLARE_DEFAULT_NO_COPY_CLASS(Semaphore);
  explicit Semaphore(VkDevice device);
};

struct Fence : NonDispatchableHandle<VkFence, VkDevice, vkDestroyFence> {
  DECLARE_DEFAULT_NO_COPY_CLASS(Fence);
  explicit Fence(VkDevice device);
};

} // namespace vk

#endif // RENDER_APIS_VK_SYNC_OBJECT_H_
