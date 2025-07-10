#ifndef RENDER_APIS_VK_COMMAND_H_
#define RENDER_APIS_VK_COMMAND_H_

#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace vk {

using CommandBuffers = NonDispatchableArrayHandle<VkCommandBuffer>;

struct CommandPool : NonDispatchableHandle<VkCommandPool, VkDevice, vkDestroyCommandPool> {
  DECLARE_DEFAULT_NO_COPY_CLASS(CommandPool);
  CommandPool(VkDevice device, uint32_t family_index);

  [[nodiscard]] CommandBuffers AllocateCommandBuffers(uint32_t count) const;
};

class Commander {
public:
  Commander(const CommandPool& command_pool, VkQueue graphics_queue);
  ~Commander() = default;

  void Begin() const;
  void End() const;
protected:
  CommandBuffers command_buffers_;
  VkQueue graphics_queue_;
};

class CommanderGuard {
public:
  explicit CommanderGuard(const Commander& commander)
    : commander_(commander) {
    commander_.Begin();
  }

  ~CommanderGuard() {
    commander_.End();
  }
private:
  const Commander& commander_;
};

} // namespace vk

#endif // RENDER_APIS_VK_COMMAND_H_
