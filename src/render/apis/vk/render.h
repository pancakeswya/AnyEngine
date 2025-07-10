#ifndef RENDER_APIS_VK_RENDER_H_
#define RENDER_APIS_VK_RENDER_H_

#include "render/apis/vk/shader.h"
#include "render/apis/vk/handle.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace vk {

struct RenderPass : NonDispatchableHandle<VkRenderPass, VkDevice, vkDestroyRenderPass> {
  DECLARE_DEFAULT_NO_COPY_CLASS(RenderPass);
  RenderPass(VkDevice device, VkFormat image_format, VkFormat depth_format);
};

struct PipelineLayout : NonDispatchableHandle<VkPipelineLayout, VkDevice, vkDestroyPipelineLayout> {
  DECLARE_DEFAULT_NO_COPY_CLASS(PipelineLayout);
  PipelineLayout(
    VkDevice device,
    const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts
  );
};

struct Pipeline : NonDispatchableHandle<VkPipeline, VkDevice, vkDestroyPipeline> {
  DECLARE_DEFAULT_NO_COPY_CLASS(Pipeline);
  Pipeline(
    VkDevice device,
    VkPipelineLayout pipeline_layout,
    VkRenderPass render_pass,
    const std::vector<VkVertexInputAttributeDescription>& attribute_descriptions,
    const std::vector<VkVertexInputBindingDescription>& binding_descriptions,
    const std::vector<Shader>& shaders
  );
};

} // namespace vk

#endif // RENDER_APIS_VK_RENDER_H_
