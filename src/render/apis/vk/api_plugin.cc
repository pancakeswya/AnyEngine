#include "render/api_plugin.h"

#include "render/apis/vk/api.h"

SDL_WindowFlags PLUGIN_CONV PluginRenderApiGetWindowFlags() {
  return SDL_WINDOW_VULKAN;
}

render::Api* PLUGIN_CONV PluginRenderApiCreate(SDL_Window* window) {
  return new vk::Api(window);
}

void PLUGIN_CONV PluginRenderApiDestroy(render::Api* api) {
  delete api;
}