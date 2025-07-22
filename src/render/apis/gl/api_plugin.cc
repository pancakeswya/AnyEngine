#include "render/api_plugin.h"

#include "render/api_plugin_internal.h"
#include "render/apis/gl/api.h"
#include "render/apis/gl/error.h"

using ApiPlugin = render::internal::ApiPlugin<gl::Api, gl::Error>;

render::Api* PLUGIN_CONV PluginRenderCreateApiAndWindow(
  const char* title,
  const int width,
  const int height,
  const SDL_WindowFlags window_flags,
  SDL_Window*& window
) {
  return ApiPlugin::CreateApiAndWindow(
    title, width, height, window_flags | SDL_WINDOW_OPENGL, window
  );
}

void PLUGIN_CONV PluginRenderDestroy(render::Api* render_api, SDL_Window* window) {
  ApiPlugin::Destroy(render_api, window);
}