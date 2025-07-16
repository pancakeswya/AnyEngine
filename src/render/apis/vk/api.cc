#include "render/apis/vk/api.h"

#include "render/apis/vk/error.h"
#include "render/apis/vk/shaders.h"

#include <array>
#include <cstring>
#include <utility>
#include <vector>
#include <limits>

namespace vk {

namespace {

inline std::vector<SwapchainImagePresent> CreateSwapchainImagePresents(
  VkDevice device,
  const Swapchain& swapchain,
  VkRenderPass render_pass,
  const ImageView& depth_image_view
) {
  const std::vector<VkImage> images = swapchain.images();
  std::vector<SwapchainImagePresent> framebuffers;
  framebuffers.reserve(images.size());
  for(VkImage image : images) {
    framebuffers.emplace_back(
      device,
      render_pass,
      image,
      depth_image_view,
      swapchain.image_info()
    );
  }
  return framebuffers;
}

template<typename SyncObjectType>
inline std::vector<SyncObjectType> CreateSyncObjects(VkDevice device, const size_t count) {
  std::vector<SyncObjectType> sync_objects;
  sync_objects.reserve(count);
  for(size_t i = 0; i < count; ++i) {
    sync_objects.emplace_back(device);
  }
  return sync_objects;
}

inline Device SelectDevice(const std::vector<VkPhysicalDevice>& physical_devices, VkSurfaceKHR surface) {
  std::vector extensions(Device::kExtensions.begin(), Device::kExtensions.end());
  std::vector layers(Instance::kLayers.begin(), Instance::kLayers.end());
  const PhysicalDevice::Requirements requirements = {
    .present = true,
    .graphic = true,
    .anisotropy = true,
    .surface = surface,
    .extensions = std::move(extensions),
    .layers = std::move(layers)
  };
  return Device::Select(physical_devices, requirements);
}

inline Swapchain CreateSwapchain(const Device& device, const Surface& surface, SDL_Window* window) {
  VkExtent2D size;
  SDL_GetWindowSize(window,
    reinterpret_cast<int*>(&size.width),
    reinterpret_cast<int*>(&size.height)
  );
  return {device, surface, size};
}

inline Image CreateDepthImage(const Device& device, const Swapchain& swapchain) {
  return {
    device,
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    VK_IMAGE_ASPECT_DEPTH_BIT,
    swapchain.image_info().extent,
    device.physical_device().FindSupportedFormat(
      {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
      },
      VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    ),
    VK_IMAGE_TILING_OPTIMAL
  };
}

VkFormat MapFormat(const SDL_PixelFormat format) noexcept {
    switch (format) {
      case SDL_PIXELFORMAT_RGBA32: return VK_FORMAT_R8G8B8A8_UNORM;
      default:
        break;
    }
    return VK_FORMAT_UNDEFINED;
}

GuiRenderer CreateGuiRender(
  SDL_Window* window,
  VkInstance instance,
  const Device& device,
  VkRenderPass render_pass,
  const uint32_t min_image_count,
  const uint32_t image_count,
  const float scale_factor
) {
  ImGui_ImplVulkan_InitInfo init_info = {
    .ApiVersion = Instance::kApiVersion,
    .Instance = instance,
    .PhysicalDevice = device.physical_device(),
    .Device = device,
    .QueueFamily = device.queues().graphics.family_index(),
    .Queue = device.queues().graphics,
    .DescriptorPoolSize = IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE,
    .RenderPass = render_pass,
    .MinImageCount = min_image_count,
    .ImageCount = image_count,
    .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
    .CheckVkResultFn =[](const VkResult result) {
      if (result != VK_SUCCESS) {
        throw Error("vk::VkResult = " + std::to_string(result));
      }
    }
  };
  return {window, scale_factor, &init_info};
}

} // namespace

Api::Api(SDL_Window* window, const float scale_factor, const size_t frame_count, const char* path)
  : instance_(path),
#ifndef NDEBUG
    debug_messenger_(instance_),
#endif
    surface_(window, instance_),
    device_(SelectDevice(instance_.devices(), surface_)),
    swapchain_(CreateSwapchain(device_, surface_, window)),
    depth_image_(CreateDepthImage(device_, swapchain_)),
    render_pass_(device_, swapchain_.image_info().format, depth_image_.info().format),
    image_presents_(CreateSwapchainImagePresents(device_, swapchain_, render_pass_, depth_image_.view())),
    acquire_semaphores_(CreateSyncObjects<Semaphore>(device_, frame_count)),
    submit_semaphores_(CreateSyncObjects<Semaphore>(device_, image_presents_.size())),
    fences_(CreateSyncObjects<Fence>(device_, frame_count)),
    command_pool_(device_, device_.queues().graphics.family_index()),
    command_buffers_(command_pool_.AllocateCommandBuffers(frame_count)),
    uniform_descriptor_set_layout_(device_),
    sampler_descriptor_set_layout_(device_),
    pipeline_layout_(device_, { uniform_descriptor_set_layout_, sampler_descriptor_set_layout_ }),
    pipeline_(device_, pipeline_layout_, render_pass_, Vertex::GetAttributeDescriptions(), Vertex::GetBindingDescriptions(), CompileShaders(device_)),
    frame_count_(frame_count),
    current_frame_(0),
    window_(window),
    gui_render_(CreateGuiRender(
      window,
      instance_,
      device_,
      render_pass_,
      swapchain_.min_image_count(),
      image_presents_.size(),
      scale_factor
    ))
{}

Api::~Api() {
  vkDeviceWaitIdle(device_);
}

void Api::OnResize([[maybe_unused]]const int width, [[maybe_unused]]const int height) {
  RecreateSwapchain(width, height);
}

void Api::RenderFrame() {
  gui_render_.RenderFrame();

  uint32_t image_idx;

  VkFence fence = fences_[current_frame_];
  VkSemaphore acquire_semaphore = acquire_semaphores_[current_frame_];
  VkCommandBuffer cmd_buffer = command_buffers_[current_frame_];

  const auto recreate_swapchain = [this]{
    int width, height;
    SDL_GetWindowSizeInPixels(window_,
      &width,
      &height
    );
    RecreateSwapchain(width, height);
  };

  if (const VkResult result = vkWaitForFences(device_, 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max()); result != VK_SUCCESS) {
    throw Error("failed to wait for fences").WithCode(result);
  }
  if (const VkResult result = vkResetFences(device_, 1, &fence); result != VK_SUCCESS) {
    throw Error("failed to reset fences").WithCode(result);
  }
  if (const VkResult result = vkAcquireNextImageKHR(device_, swapchain_, std::numeric_limits<uint64_t>::max(), acquire_semaphore, VK_NULL_HANDLE, &image_idx); result != VK_SUCCESS) {
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
      recreate_swapchain();
      return;
    }
    throw Error("failed to acquire next image").WithCode(result);
  }
  VkSemaphore submit_semaphore = submit_semaphores_[image_idx];
  if (const VkResult result = vkResetCommandBuffer(cmd_buffer, 0); result != VK_SUCCESS) {
    throw Error("failed to reset command buffer").WithCode(result);
  }
  RecordCommandBuffer(cmd_buffer, image_idx);

  constexpr std::array<VkPipelineStageFlags, 1> pipeline_stages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  const VkSubmitInfo submit_info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &acquire_semaphore,
    .pWaitDstStageMask = pipeline_stages.data(),
    .commandBufferCount = 1,
    .pCommandBuffers = &cmd_buffer,
    .signalSemaphoreCount = 1,
    .pSignalSemaphores = &submit_semaphore
  };
  if (const VkResult result = vkQueueSubmit(device_.queues().graphics, 1, &submit_info, fence); result != VK_SUCCESS) {
    throw Error("failed to submit draw command buffer").WithCode(result);
  }
  std::array<VkSwapchainKHR, 1> swapchains = {swapchain_};
  const VkPresentInfoKHR present_info = {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &submit_semaphore,
    .swapchainCount = 1,
    .pSwapchains = swapchains.data(),
    .pImageIndices = &image_idx
  };
  if (const VkResult result = vkQueuePresentKHR(device_.queues().present, &present_info);
      result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    recreate_swapchain();
  } else if (result != VK_SUCCESS) {
    throw Error("failed to queue present").WithCode(result);
  }
  current_frame_ = (current_frame_ + 1) % frame_count_;
  for(Object& object : objects_) {
    object.SetFrameIndex(current_frame_);
  }
}

template <VkBufferUsageFlagBits usage>
inline StagingBuffer Api::TransferBufferToStaging(
    const TransferBuffer& transfer_buffer) const {
  StagingBuffer staging_buffer(device_, usage, transfer_buffer.size());
  BufferCommander commander(staging_buffer, command_pool_, device_.queues().graphics);
  CommanderGuard commander_guard(commander);
  commander.CopyBuffer(transfer_buffer);
  return staging_buffer;
}

render::Object* Api::LoadObject(
    render::GeometryTransferer& geometry_transferer,
    std::vector<std::unique_ptr<render::TextureMapper>>& texture_mappers
) {
  const render::GeometryInfo geometry_info = geometry_transferer.info();
  const std::vector<render::TextureIndices>& texture_indices = geometry_transferer.texture_indices();

  const TransferBuffer transfer_vertices(device_, sizeof(Vertex) * geometry_info.index_count);
  const TransferBuffer transfer_indices(device_, sizeof(Index) * geometry_info.index_count);

  const auto vertices = transfer_vertices.memory().Map<Vertex>();
  const auto indices = transfer_indices.memory().Map<Index>();

  geometry_transferer.Transfer(vertices, indices);

  transfer_vertices.memory().Unmap();
  transfer_indices.memory().Unmap();

  StagingBuffer staging_vertices = TransferBufferToStaging<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>(transfer_vertices);
  StagingBuffer staging_indices = TransferBufferToStaging<VK_BUFFER_USAGE_INDEX_BUFFER_BIT>(transfer_indices);

  std::vector<StagingImage> images;
  images.reserve(texture_mappers.size());
  for (const std::unique_ptr<render::TextureMapper>& texture_mapper : texture_mappers) {
    const auto[width, height, format] = texture_mapper->info();

    const SDL_PixelFormat sdl_format = MapFormat(format) == VK_FORMAT_UNDEFINED ? SDL_PIXELFORMAT_RGBA32 : format;
    const VkFormat vk_format = MapFormat(sdl_format);

    const size_t pixel_count = width * height * SDL_BYTESPERPIXEL(sdl_format);

    const TransferBuffer transfer_texture(device_, pixel_count);

    const uint8_t* pixels = texture_mapper->Map(sdl_format);
    const auto pixels_mapped = transfer_texture.memory().Map<uint8_t>();;
    std::memcpy(pixels_mapped, pixels, pixel_count);

    transfer_texture.memory().Unmap();

    const VkExtent2D extent = {
      .width = static_cast<uint32_t>(width),
      .height = static_cast<uint32_t>(height),
    };
    StagingImage image(device_, extent, vk_format);

    ImageCommander commander(image, command_pool_, device_.queues().graphics);

    CommanderGuard commander_guard(commander);
    commander.TransitImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    commander.CopyBuffer(transfer_texture);
    commander.GenerateMipmaps();

    images.emplace_back(std::move(image));
  }
  objects_.emplace_back(
    device_,
    uniform_descriptor_set_layout_,
    sampler_descriptor_set_layout_,
    geometry_info,
    texture_indices,
    std::move(staging_vertices),
    std::move(staging_indices),
    std::move(images),
    frame_count_
  );
  return objects_.data() + objects_.size() - 1;
}

void Api::RecreateSwapchain(const int width, const int height) {
  if (const VkResult result = vkDeviceWaitIdle(device_); result != VK_SUCCESS) {
    throw Error("failed to idle device");
  }
  const VkExtent2D extent = {
    .width = static_cast<uint32_t>(width),
    .height = static_cast<uint32_t>(height),
  };
  swapchain_ = Swapchain(device_, surface_, extent, swapchain_);
  depth_image_ = CreateDepthImage(device_, swapchain_);
  image_presents_ = CreateSwapchainImagePresents(device_, swapchain_, render_pass_, depth_image_.view());
  
  acquire_semaphores_= CreateSyncObjects<Semaphore>(device_, frame_count_);
  submit_semaphores_ = CreateSyncObjects<Semaphore>(device_, image_presents_.size());
  fences_ = CreateSyncObjects<Fence>(device_, frame_count_);
}

void Api::RecordCommandBuffer(VkCommandBuffer cmd_buffer, const size_t image_idx) const {
  constexpr VkCommandBufferBeginInfo cmd_buffer_begin_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
  };
  if (const VkResult result = vkBeginCommandBuffer(cmd_buffer, &cmd_buffer_begin_info);
      result != VK_SUCCESS) {
    throw Error("failed to begin recording command buffer").WithCode(result);
  }
  constexpr std::array clear_values = {
    VkClearValue{.color = {{0.0f, 0.0f, 0.0f, 1.0f}}},
    VkClearValue{.depthStencil = {1.0f, 0}}
  };
  const VkRenderPassBeginInfo render_pass_begin_info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .renderPass = render_pass_,
    .framebuffer = image_presents_[image_idx].framebuffer(),
    .renderArea = {
        .offset = {0, 0},
        .extent = swapchain_.image_info().extent
    },
    .clearValueCount = static_cast<uint32_t>(clear_values.size()),
    .pClearValues = clear_values.data()
  };
  vkCmdBeginRenderPass(cmd_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);

  const VkExtent2D extent = swapchain_.image_info().extent;
  const VkViewport viewport = {
    .x = 0.0f,
    .y = static_cast<float>(extent.height),
    .width = static_cast<float>(extent.width),
    .height = -static_cast<float>(extent.height),
    .minDepth = 0.0f,
    .maxDepth = 1.0f
  };
  vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);

  const VkRect2D scissor = {
    .offset = {0, 0},
    .extent = extent
  };
  vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);

  for(const Object& object : objects_) {
    VkBuffer vertices_buffer = object.vertices();
    VkBuffer indices_buffer = object.indices();
    VkDescriptorSet uniform_descriptor_set = object.uniform_descriptor_sets().at(current_frame_);

    VkDeviceSize prev_offset = 0;
    constexpr std::array<VkDeviceSize, 1> vertex_offsets = {};

    vkCmdBindVertexBuffers(cmd_buffer, 0, vertex_offsets.size(), &vertices_buffer, vertex_offsets.data());
    vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout_, 0, 1, &uniform_descriptor_set, 0, nullptr);

    for(const auto[index, offset] : object.texture_indices()) {
      VkDescriptorSet sampler_descriptor_set = object.sampler_descriptor_sets().at(index);
      vkCmdBindIndexBuffer(cmd_buffer, indices_buffer, prev_offset * sizeof(Index), IndexType<Index>::value);
      vkCmdBindDescriptorSets(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout_, 1, 1, &sampler_descriptor_set, 0, nullptr);
      vkCmdDrawIndexed(cmd_buffer, static_cast<uint32_t>(offset - prev_offset), 1, 0, 0, 0);

      prev_offset = offset;
    }
  }
  GuiRenderer::Record(cmd_buffer);
  vkCmdEndRenderPass(cmd_buffer);
  if (const VkResult result = vkEndCommandBuffer(cmd_buffer); result != VK_SUCCESS) {
    throw Error("failed to record command buffer").WithCode(result);
  }
}

} // namespace vk