#ifndef RENDER_APIS_VK_WINDOW_H_
#define RENDER_APIS_VK_WINDOW_H_

#include <vulkan/vulkan.h>

namespace vk {

struct Window {
  virtual ~Window() = default;
  [[nodiscard]] virtual VkExtent2D size() const noexcept = 0;
};

} // namespace vk

#endif // RENDER_APIS_VK_WINDOW_H_
