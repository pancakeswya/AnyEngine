#include "render/api_plugin.h"

#include "render/apis/gl/api.h"

SDL_WindowFlags PLUGIN_CONV PluginRenderApiGetWindowFlags() {
  return SDL_WINDOW_OPENGL;
}

render::Api* PLUGIN_CONV PluginRenderApiCreate(SDL_Window* window, const float scale_factor) {
  return new gl::Api(window, scale_factor);
}

void PLUGIN_CONV PluginRenderApiDestroy(render::Api* api) {
  delete api;
}