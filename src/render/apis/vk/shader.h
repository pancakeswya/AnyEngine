#ifndef RENDER_APIS_VK_SHADER_H_
#define RENDER_APIS_VK_SHADER_H_

#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

#include <string_view>
#include <vector>

namespace vk {

class Shader : public NonDispatchableHandle<VkShaderModule, VkDevice, vkDestroyShaderModule> {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Shader);
  Shader(
    VkDevice device,
    const std::vector<uint32_t>& spirv,
    VkShaderStageFlagBits stage,
    std::string_view entry_point
  );

  [[nodiscard]] VkShaderStageFlagBits stage() const noexcept {
    return stage_;
  }

  [[nodiscard]] std::string_view entry_point() const noexcept {
    return entry_point_;
  }
private:
  VkShaderStageFlagBits stage_ = {};
  std::string_view entry_point_;
};

} // namespace vk

#endif //RENDER_APIS_VK_SHADER_H_