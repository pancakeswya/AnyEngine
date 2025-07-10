#ifndef RENDER_APIS_VK_BUFFER_H_
#define RENDER_APIS_VK_BUFFER_H_

#include "render/apis/vk/device.h"
#include "render/apis/vk/command.h"
#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

namespace vk {

struct BufferMemory : DeviceMemory {
  DECLARE_DEFAULT_NO_COPY_CLASS(BufferMemory);
  BufferMemory(
    const Device& device,
    VkBuffer buffer,
    VkMemoryPropertyFlags properties
  );
};

class Buffer : public NonDispatchableHandle<VkBuffer, VkDevice, vkDestroyBuffer> {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Buffer);
  Buffer(
    const Device& device,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkDeviceSize size
  );
  [[nodiscard]] const BufferMemory& memory() const noexcept {
    return memory_;
  }

  [[nodiscard]] VkDeviceSize size() const noexcept {
    return size_;
  }
private:
  BufferMemory memory_;
  VkDeviceSize size_ = 0;
};

class BufferCommander : public Commander {
public:
  BufferCommander(Buffer& buffer, const CommandPool& command_pool, VkQueue graphics_queue);
  ~BufferCommander() = default;

  void CopyBuffer(const Buffer& src) const;
private:
  Buffer& buffer_;
};

struct TransferBuffer : Buffer {
  DECLARE_DEFAULT_NO_COPY_CLASS(TransferBuffer);
  TransferBuffer(const Device& device, VkDeviceSize size);
};

struct StagingBuffer : Buffer {
  DECLARE_DEFAULT_NO_COPY_CLASS(StagingBuffer);
  StagingBuffer(
    const Device& device,
    VkBufferUsageFlags usage,
    VkDeviceSize size
  );
};

} // namespace vk

#endif // RENDER_APIS_VK_BUFFER_H_
