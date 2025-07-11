#include "render/apis/vk/queue.h"

namespace vk {

namespace {

inline VkQueue GetQueue(VkDevice device, const uint32_t family_index) {
  VkQueue queue;
  vkGetDeviceQueue(device, family_index, 0, &queue);
  return queue;
}

} // namespace

Queue::Queue(VkDevice device, const uint32_t family_index) noexcept
  : Handle(GetQueue(device, family_index)),
    family_index_(family_index) {}


} // namespace vk