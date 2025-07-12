#include "render/apis/vk/command.h"

#include "render/apis/vk/error.h"

namespace vk {

namespace {

inline VkCommandPool CreateCommandPool(VkDevice device, const uint32_t family_index) {
  const VkCommandPoolCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = family_index
  };
  VkCommandPool pool;
  if (const VkResult result = vkCreateCommandPool(device, &info, nullptr, &pool); result != VK_SUCCESS) {
    throw Error("failed to create buffer").WithCode(result);
  }
  return pool;
}

} // namespace

CommandPool::CommandPool(VkDevice device, const uint32_t family_index)
  : NonDispatchableHandle(CreateCommandPool(device, family_index), device) {}

CommandBuffers CommandPool::AllocateCommandBuffers(const uint32_t count) const {
  const VkCommandBufferAllocateInfo info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .commandPool = handle_,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = count
  };
  const auto command_buffers = new VkCommandBuffer[count];
  if (const VkResult result = vkAllocateCommandBuffers(parent_, &info, command_buffers); result != VK_SUCCESS) {
    delete[] command_buffers;
    throw Error("failed to allocate command buffers").WithCode(result);
  }
  return {command_buffers, [device = parent_, pool = handle_, count](VkCommandBuffer command_buffers[]) {
    vkFreeCommandBuffers(device, pool, count, command_buffers);
    delete[] command_buffers;
  }};
}

Commander::Commander(const CommandPool& command_pool, VkQueue graphics_queue)
  : command_buffers_(command_pool.AllocateCommandBuffers(1)),
    graphics_queue_(graphics_queue) {}

void Commander::Begin() const {
  constexpr VkCommandBufferBeginInfo begin_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
  };
  if (const VkResult result = vkBeginCommandBuffer(command_buffers_[0], &begin_info); result != VK_SUCCESS) {
    throw Error("failed to begin command buffer").WithCode(result);
  }
}

void Commander::End() const {
  if (const VkResult result = vkEndCommandBuffer(command_buffers_[0]); result != VK_SUCCESS) {
    throw Error("failed to end cmd buffer").WithCode(result);
  }
  const VkSubmitInfo submit_info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .commandBufferCount = 1,
    .pCommandBuffers = command_buffers_.get(),
  };
  if (const VkResult result = vkQueueSubmit(graphics_queue_, 1, &submit_info, VK_NULL_HANDLE); result != VK_SUCCESS) {
    throw Error("failed to submin the graphics queue");
  }
  if (const VkResult result = vkQueueWaitIdle(graphics_queue_); result != VK_SUCCESS) {
    throw Error("failed to wait idle graphics queue").WithCode(result);
  }
}

} // namespace vk