#ifndef RENDER_API_PLUGIN_H_
#define RENDER_API_PLUGIN_H_

#include "base/plugin_defs.h"
#include "render/api.h"

#include <SDL3/SDL_video.h>

extern "C" {

extern PLUGIN_API render::Api* PluginRenderCreateApiAndWindow(
  const char* title,
  int width,
  int height,
  SDL_WindowFlags window_flags,
  SDL_Window*& window
);

extern PLUGIN_API void PluginRenderDestroy(render::Api* render_api, SDL_Window* window);

} // extern "C"

#endif // RENDER_API_PLUGIN_H_
