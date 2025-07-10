#ifndef RENDER_VK_SHADERS_H_
#define RENDER_VK_SHADERS_H_

#include "render/apis/vk/shader.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace vk {

extern std::vector<Shader> CompileShaders(VkDevice device);

} // namespace vk

#endif // RENDER_VK_SHADERS_H_
