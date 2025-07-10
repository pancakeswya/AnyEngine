#include "render/api_plugin.h"

#include "render/apis/vk/api.h"

PLUGIN_CONV SDL_WindowFlags PluginRenderApiGetWindowFlags() {
  return SDL_WINDOW_VULKAN;
}

PLUGIN_CONV render::Api* PluginRenderApiCreate(SDL_Window* window) {
  return new vk::Api(window);
}

PLUGIN_CONV void PluginRenderApiDestroy(render::Api* api) {
  delete api;
}