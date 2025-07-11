#ifndef RENDER_APIS_VK_API_H_
#define RENDER_APIS_VK_API_H_

#include "render/api.h"
#include "render/apis/vk/instance.h"
#include "render/apis/vk/surface.h"
#include "render/apis/vk/device.h"
#include "render/apis/vk/swapchain.h"
#include "render/apis/vk/image.h"
#include "render/apis/vk/render.h"
#include "render/apis/vk/sync.h"
#include "render/apis/vk/command.h"
#include "render/apis/vk/object.h"

#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace vk {

class Api final : public render::Api {
public:
  explicit Api(SDL_Window* window, size_t frame_count = 2, const char* path = nullptr);
  ~Api() override;

  void OnResize(int width, int height) override;

  void RenderFrame() override;
  render::Object& LoadObject(
      render::GeometryTransferer& geometry_transferer,
      std::vector<std::unique_ptr<render::TextureTransferer>>&
          texture_transferers) override;
private:
  template<VkBufferUsageFlagBits usage>
  [[nodiscard]] StagingBuffer TransferBufferToStaging(const TransferBuffer& transfer_buffer) const;

  void RecreateSwapchain();
  void RecordCommandBuffer(VkCommandBuffer cmd_buffer, size_t image_idx) const;

  Instance instance_;
#ifndef NDEBUG
  DebugMessenger debug_messenger_;
#endif
  Surface surface_;
  Device device_;
  Swapchain swapchain_;
  Image depth_image_;
  RenderPass render_pass_;

  std::vector<SwapchainFramebuffer> framebuffers_;
  std::vector<SyncObject> sync_objects_;

  CommandPool command_pool_;
  CommandBuffers command_buffers_;

  std::vector<Object> objects_;

  UniformDescriptorSetLayout uniform_descriptor_set_layout_;
  SamplerDescriptorSetLayout sampler_descriptor_set_layout_;
  PipelineLayout pipeline_layout_;
  Pipeline pipeline_;

  size_t frame_count_;
  size_t current_frame_;
  bool framebuffer_resized_;
  SDL_Window* window_;
};

} // namespace vk

#endif // RENDER_APIS_VK_API_H_
