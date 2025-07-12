#include "render/apis/vk/sync.h"

#include "render/apis/vk/error.h"

namespace vk {

namespace {

inline VkSemaphore CreateSemaphore(VkDevice device) {
  constexpr VkSemaphoreCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
  };
  VkSemaphore semaphore;
  if (const VkResult result = vkCreateSemaphore(device, &info, nullptr, &semaphore); result != VK_SUCCESS) {
    throw Error("failed to create semaphore").WithCode(result);
  }
  return semaphore;
}

inline VkFence CreateFence(VkDevice device) {
  constexpr VkFenceCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .flags = VK_FENCE_CREATE_SIGNALED_BIT
  };
  VkFence fence;
  if (const VkResult result = vkCreateFence(device, &info, nullptr, &fence); result != VK_SUCCESS) {
    throw Error("failed to create semaphore").WithCode(result);
  }
  return fence;
}

} // namespace

Semaphore::Semaphore(VkDevice device) : NonDispatchableHandle(CreateSemaphore(device), device) {}

Fence::Fence(VkDevice device) : NonDispatchableHandle(CreateFence(device), device) {}

} // namespace vk