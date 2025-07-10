#ifndef RENDER_APIS_VK_QUEUE_H_
#define RENDER_APIS_VK_QUEUE_H_

#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

namespace vk {

class Queue : public Handle<VkQueue, NoDestroy<VkQueue>> {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Queue);
  Queue(VkDevice device, uint32_t family_index) noexcept;

  [[nodiscard]] uint32_t family_index() const noexcept {
    return family_index_;
  }
private:
  uint32_t family_index_ = 0;
};

} // namespace vk

#endif // RENDER_APIS_VK_QUEUE_H_
