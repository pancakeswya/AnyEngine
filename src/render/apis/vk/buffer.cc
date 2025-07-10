#include "render/apis/vk/buffer.h"

#include "render/apis/vk/error.h"

#include <utility>

namespace vk {

namespace {

VkBuffer CreateBuffer(VkDevice device, const VkBufferUsageFlags usage, const VkDeviceSize size) {
  const VkBufferCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    .size = size,
    .usage = usage,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE
  };
  VkBuffer buffer;
  if (const VkResult result = vkCreateBuffer(device, &info, nullptr, &buffer); result != VK_SUCCESS) {
    throw Error("failed to create buffer").WithCode(result);
  }
  return buffer;
}

inline VkMemoryRequirements GetMemoryRequirements(VkDevice device, VkBuffer buffer) {
  VkMemoryRequirements requirements;
  vkGetBufferMemoryRequirements(device, buffer, &requirements);
  return requirements;
}

} // namespace

BufferMemory::BufferMemory(
  const Device& device,
  VkBuffer buffer,
  const VkMemoryPropertyFlags properties
) : DeviceMemory(device, properties, GetMemoryRequirements(device, buffer)) {
  if (const VkResult result = vkBindBufferMemory(device, buffer, *this, 0); result != VK_SUCCESS) {
    throw Error("failed to bind buffer memory").WithCode(result);
  }
}

Buffer::Buffer(
  const Device& device,
  const VkBufferUsageFlags usage,
  const VkMemoryPropertyFlags properties,
  const VkDeviceSize size
) : NonDispatchableHandle(CreateBuffer(device, usage, size), device),
    memory_(device, handle_, properties),
    size_(size) {}

BufferCommander::BufferCommander(Buffer& buffer, const CommandPool& command_pool, VkQueue graphics_queue)
  : Commander(command_pool, graphics_queue),
    buffer_(buffer) {}

void BufferCommander::CopyBuffer(const Buffer& src) const {
  const VkBufferCopy copy_region = {
    .size = src.size(),
  };
  vkCmdCopyBuffer(command_buffers_[0], src, buffer_, 1, &copy_region);
}

TransferBuffer::TransferBuffer(const Device& device, VkDeviceSize size)
  : Buffer(device,
           VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
           size) {}

StagingBuffer::StagingBuffer(
  const Device& device,
  const VkBufferUsageFlags usage,
  const VkDeviceSize size
) : Buffer(device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, size) {}

} // namespace vk