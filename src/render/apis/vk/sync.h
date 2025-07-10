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

class SyncObject {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(SyncObject);
  explicit SyncObject(VkDevice device);

  [[nodiscard]] VkSemaphore image_semaphore() const noexcept {
    return image_semaphore_;
  }

  [[nodiscard]] VkSemaphore render_semaphore() const noexcept {
    return render_semaphore_;
  }

  [[nodiscard]] VkFence fence() const noexcept {
    return fence_;
  }
private:
  Semaphore image_semaphore_;
  Semaphore render_semaphore_;
  Fence fence_;
};

} // namespace vk

#endif // RENDER_APIS_VK_SYNC_OBJECT_H_
