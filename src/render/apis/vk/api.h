#ifndef RENDER_APIS_VK_API_H_
#define RENDER_APIS_VK_API_H_

#include "render/apis/vk/instance.h"
#include "render/apis/vk/surface.h"
#include "render/apis/vk/device.h"
#include "render/apis/vk/swapchain.h"
#include "render/apis/vk/image.h"
#include "render/apis/vk/render.h"
#include "render/apis/vk/sync.h"
#include "render/apis/vk/command.h"
#include "render/apis/vk/object.h"
#include "render/apis/vk/texture_mapper.h"
#include "render/apis/vk/window.h"
#include "render/api.h"

#include <vulkan/vulkan.h>

#include <cstddef>
#include <vector>
#include <memory>
#include <functional>

namespace vk {

class Api final : public render::Api<TextureMapper> {
public:
  using RecordFunc = void(*)(VkCommandBuffer command_buffer);

  explicit Api(
    const Instance& instance,
    const Window& window,
    const Surface& surface,
    RecordFunc record_func,
    size_t frame_count = 2
  );
  ~Api() override = default;

  void OnResize(int width, int height) override;

  void RenderFrame() override;

  render::Object* LoadObject(
      render::GeometryTransferer& geometry_transferer,
      std::vector<std::unique_ptr<TextureMapper>>& texture_mappers
  ) override;

  [[nodiscard]] const Device& device() const noexcept { return device_; }
  [[nodiscard]] const Swapchain& swapchain() const noexcept { return swapchain_; }
  [[nodiscard]] const RenderPass& render_pass() const noexcept { return render_pass_; }
  [[nodiscard]] const std::vector<SwapchainImagePresent>& image_presents() const noexcept { return image_presents_; }
private:
  void RecreateSwapchain(VkExtent2D extent);
  VkCommandBuffer RecordCommandBuffer(size_t image_idx);

  Device device_;
  Swapchain swapchain_;
  Image depth_image_;
  RenderPass render_pass_;

  std::vector<SwapchainImagePresent> image_presents_;
  std::vector<Semaphore> acquire_semaphores_;
  std::vector<Semaphore> submit_semaphores_;
  std::vector<Fence> fences_;

  CommandPool command_pool_;
  CommandBuffers command_buffers_;

  std::vector<Object> objects_;

  UniformDescriptorSetLayout uniform_descriptor_set_layout_;
  SamplerDescriptorSetLayout sampler_descriptor_set_layout_;
  PipelineLayout pipeline_layout_;
  Pipeline pipeline_;

  size_t frame_count_;
  size_t current_frame_;

  const Window& window_;
  const Surface& surface_;
  RecordFunc record_func_;
};

} // namespace vk

#endif // RENDER_APIS_VK_API_H_
