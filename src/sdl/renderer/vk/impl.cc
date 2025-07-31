#include "sdl/renderer/vk/impl.h"

#include "sdl/image/convert.h"
#include "sdl/renderer/vk/error.h"
#include "sdl/renderer/vk/texture_mapper.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_vulkan.h>

namespace sdl::vk {

namespace {

::vk::Instance CreateInstance() {
  if (!SDL_Vulkan_LoadLibrary(nullptr)) {
    throw Error("sdl load vulkan failed").WithMessage();
  }
  Uint32 extension_count = 0;
  char const* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extension_count);
  if (extensions == nullptr) {
    throw Error("SDL_Vulkan_GetInstanceExtensions failed").WithMessage();
  }
  return {
    extensions,
    extension_count
#ifndef ANY_RELEASE
  , SDL_Log
#endif
  };
}

::vk::Surface CreateSurface(const Window& window, const ::vk::Instance& instance) {
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
    throw Error("SDL_Vulkan_CreateSurface failed").WithMessage();
  }
  return {surface, instance};
}

GuiRenderer CreateGuiRenderer(
  const Window& window,
  const ::vk::Instance& instance,
  const ::vk::Device& device,
  const ::vk::Swapchain& swapchain,
  const ::vk::RenderPass& render_pass,
  const uint32_t image_count,
  const float scale_factor) {
  ImGui_ImplVulkan_InitInfo init_info = {
    .ApiVersion = ::vk::Instance::kApiVersion,
    .Instance = instance,
    .PhysicalDevice = device.physical_device(),
    .Device = device,
    .QueueFamily = device.queues().graphics.family_index(),
    .Queue = device.queues().graphics,
    .RenderPass = render_pass,
    .MinImageCount = swapchain.min_image_count(),
    .ImageCount = image_count,
    .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
    .DescriptorPoolSize = IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE,
    .CheckVkResultFn =[](const VkResult result) {
      if (result != VK_SUCCESS) {
        throw Error("vk::VkResult = " + std::to_string(result));
      }
    }
  };
  return {window, scale_factor, &init_info};
}

} // namespace

RendererImpl::RendererImpl(SDL_Window* window, const float scale_factor)
  : window_(window),
    instance_(CreateInstance()),
    surface_(CreateSurface(window_, instance_)),
    renderer_(
      instance_,
      window_,
      surface_,
      GuiRenderer::Record
    ),
    gui_renderer_(
      CreateGuiRenderer(
        window_,
        instance_,
        renderer_.device(),
        renderer_.swapchain(),
        renderer_.render_pass(),
        renderer_.image_presents().size(),
        scale_factor
      )
    ) {}

RendererImpl::~RendererImpl() {
  vkDeviceWaitIdle(renderer_.device());
}

void RendererImpl::RenderFrame() {
  gui_renderer_.RenderFrame();
  renderer_.RenderFrame();
}

render::Object* RendererImpl::LoadObject(
    render::GeometryTransferer& geometry_transferer,
    const std::vector<std::string>& texture_paths) {
  std::vector texture_mappers = ConvertPathsToMappers<::vk::TextureMapper, TextureMapper, TextureMapperMock>(texture_paths);
  return renderer_.LoadObject(geometry_transferer, texture_mappers);
}

void RendererImpl::OnResize(const int width, const int height) {
  renderer_.OnResize(width, height);
}

} // namespace sdl::vk