#include "render/api_plugin.h"

#include "render/apis/gl/api.h"

SDL_WindowFlags PLUGIN_CONV PluginRenderApiGetWindowFlags() {
  return SDL_WINDOW_OPENGL;
}

render::Api* PLUGIN_CONV PluginRenderApiCreate(SDL_Window* window) {
  return new gl::Api(window);
}

void PLUGIN_CONV PluginRenderApiDestroy(render::Api* api) {
  delete api;
}