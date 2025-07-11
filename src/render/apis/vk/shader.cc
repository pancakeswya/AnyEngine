#include "render/apis/vk/shader.h"

#include "render/apis/vk/error.h"

namespace vk {

namespace {

inline VkShaderModule CreateShaderModule(VkDevice device, const std::vector<uint32_t>& spirv) {
  const VkShaderModuleCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .codeSize = spirv.size() * sizeof(uint32_t),
    .pCode = spirv.data()
  };
  VkShaderModule shader;
  if (const VkResult result = vkCreateShaderModule(device, &info, nullptr, &shader); result != VK_SUCCESS) {
    throw Error("failed to create sampler").WithCode(result);
  }
  return shader;
}

} // namespace

Shader::Shader(
  VkDevice device,
  const std::vector<uint32_t>& spirv,
  const VkShaderStageFlagBits stage,
  const std::string_view entry_point
) : NonDispatchableHandle(CreateShaderModule(device, spirv), device),
    stage_(stage),
    entry_point_(entry_point) {}

} // namespace vk