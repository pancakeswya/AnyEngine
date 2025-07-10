#ifndef RENDER_APIS_VK_ERROR_H_
#define RENDER_APIS_VK_ERROR_H_

#include "render/error.h"

#include <vulkan/vulkan.h>

#include <string>

namespace vk {

struct Error final : render::Error {
  using render::Error::Error;

  [[nodiscard]] Error WithCode(const VkResult result) const {
    return Error{std::string(what()) + " [Code: " + std::to_string(result) + ']'};
  }
};

} // namespace vk

#endif // RENDER_APIS_VK_ERROR_H_
