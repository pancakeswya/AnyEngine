#include "render/apis/vk/render.h"

#include "render/apis/vk/error.h"

#include <array>
#include <algorithm>

namespace vk {

namespace {

VkRenderPass CreateRenderPass(VkDevice device, const VkFormat image_format, const VkFormat depth_format) {
  const VkAttachmentDescription color_attachment = {
    .format = image_format,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
  };
  const VkAttachmentDescription depth_attachment = {
      .format = depth_format,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
  };
  constexpr VkAttachmentReference color_attachment_ref = {
      .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
  };
  constexpr VkAttachmentReference depth_attachment_ref = {
      .attachment = 1,
      .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
  };
  VkSubpassDescription subpass = {
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .colorAttachmentCount = 1,
      .pColorAttachments = &color_attachment_ref,
      .pDepthStencilAttachment = &depth_attachment_ref
  };
  constexpr VkSubpassDependency dependency = {
    .srcSubpass = VK_SUBPASS_EXTERNAL,
    .dstSubpass = 0,
    .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    .srcAccessMask = 0,
    .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
  };
  const std::array attachments = {color_attachment, depth_attachment};

  const VkRenderPassCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .attachmentCount = static_cast<uint32_t>(attachments.size()),
    .pAttachments = attachments.data(),
    .subpassCount = 1,
    .pSubpasses = &subpass,
    .dependencyCount = 1,
    .pDependencies = &dependency
  };
  VkRenderPass render_pass;
  if (const VkResult result = vkCreateRenderPass(device, &info, nullptr, &render_pass); result != VK_SUCCESS) {
    throw Error("failed to create render pass").WithCode(result);
  }
  return render_pass;
}

inline VkPipelineLayout CreatePipelineLayout(VkDevice device, const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts) {
  const VkPipelineLayoutCreateInfo info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts.size()),
    .pSetLayouts = descriptor_set_layouts.data()
  };
  VkPipelineLayout pipeline_layout;
  if (const VkResult result = vkCreatePipelineLayout(device, &info, nullptr, &pipeline_layout); result != VK_SUCCESS) {
    throw Error("failed to create pipeline layout").WithCode(result);
  }
  return pipeline_layout;
}

VkPipeline CreatePipeline(
  VkDevice device,
  VkPipelineLayout pipeline_layout,
  VkRenderPass render_pass,
  const std::vector<VkVertexInputAttributeDescription>& attribute_descriptions,
  const std::vector<VkVertexInputBindingDescription>& binding_descriptions,
  const std::vector<Shader>& shaders
) {
  std::vector<VkPipelineShaderStageCreateInfo> shader_stages_infos;
  shader_stages_infos.reserve(shaders.size());
  for(const Shader& shader : shaders) {
    const VkPipelineShaderStageCreateInfo shader_stage_info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = shader.stage(),
      .module = shader,
      .pName = shader.entry_point().data(),
    };
    shader_stages_infos.push_back(shader_stage_info);
  }
  const VkPipelineVertexInputStateCreateInfo vertex_input_info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount =
          static_cast<uint32_t>(binding_descriptions.size()),
      .pVertexBindingDescriptions = binding_descriptions.data(),
      .vertexAttributeDescriptionCount =
          static_cast<uint32_t>(attribute_descriptions.size()),
      .pVertexAttributeDescriptions = attribute_descriptions.data()
  };
  constexpr VkPipelineInputAssemblyStateCreateInfo input_assembly = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitiveRestartEnable = VK_FALSE
  };
  constexpr VkPipelineViewportStateCreateInfo viewport_state = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .viewportCount = 1,
      .scissorCount = 1
  };
  constexpr VkPipelineRasterizationStateCreateInfo rasterizer = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .depthClampEnable = VK_FALSE,
    .rasterizerDiscardEnable = VK_FALSE,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_FRONT_BIT,
    .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    .depthBiasEnable = VK_FALSE,
    .lineWidth = 1.0f
  };
  constexpr VkPipelineMultisampleStateCreateInfo multisampling = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_FALSE
  };
  constexpr VkPipelineDepthStencilStateCreateInfo depth_stencil = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
      .depthTestEnable = VK_TRUE,
      .depthWriteEnable = VK_TRUE,
      .depthCompareOp = VK_COMPARE_OP_LESS,
      .depthBoundsTestEnable = VK_FALSE,
      .stencilTestEnable = VK_FALSE
  };
  static constexpr VkPipelineColorBlendAttachmentState color_blend_attachment = {
      .blendEnable = VK_FALSE,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
  };
  constexpr VkPipelineColorBlendStateCreateInfo color_blending = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .logicOpEnable = VK_FALSE,
    .logicOp = VK_LOGIC_OP_COPY,
    .attachmentCount = 1,
    .pAttachments = &color_blend_attachment,
    .blendConstants = {}
  };
  static constexpr std::array dynamic_states = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
  };
  constexpr VkPipelineDynamicStateCreateInfo dynamic_state = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
    .dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
    .pDynamicStates = dynamic_states.data()
  };
  const VkGraphicsPipelineCreateInfo pipeline_info = {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .stageCount = 2,
    .pStages = shader_stages_infos.data(),
    .pVertexInputState = &vertex_input_info,
    .pInputAssemblyState = &input_assembly,
    .pViewportState = &viewport_state,
    .pRasterizationState = &rasterizer,
    .pMultisampleState = &multisampling,
    .pDepthStencilState = &depth_stencil,
    .pColorBlendState = &color_blending,
    .pDynamicState = &dynamic_state,
    .layout = pipeline_layout,
    .renderPass = render_pass,
    .subpass = 0,
    .basePipelineHandle = VK_NULL_HANDLE
  };
  VkPipeline pipeline = VK_NULL_HANDLE;
  if (const VkResult result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline); result != VK_SUCCESS) {
    throw Error("failed to create graphics pipeline").WithCode(result);
  }
  return pipeline;
}

} // namespace

RenderPass::RenderPass(VkDevice device, VkFormat image_format, VkFormat depth_format)
  : NonDispatchableHandle(CreateRenderPass(device, image_format, depth_format), device) {}

PipelineLayout::PipelineLayout(
  VkDevice device,
  const std::vector<VkDescriptorSetLayout>& descriptor_set_layouts
) : NonDispatchableHandle(CreatePipelineLayout(device, descriptor_set_layouts), device) {}

Pipeline::Pipeline(
  VkDevice device,
  VkPipelineLayout pipeline_layout,
  VkRenderPass render_pass,
  const std::vector<VkVertexInputAttributeDescription>& attribute_descriptions,
  const std::vector<VkVertexInputBindingDescription>& binding_descriptions,
  const std::vector<Shader>& shaders
) : NonDispatchableHandle(CreatePipeline(device, pipeline_layout, render_pass, attribute_descriptions, binding_descriptions, shaders), device) {}

} // namespace vk