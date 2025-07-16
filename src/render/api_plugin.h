#ifndef RENDER_API_PLUGIN_H_
#define RENDER_API_PLUGIN_H_

#include "base/plugin_defs.h"
#include "render/api.h"

#include <SDL3/SDL.h>

extern "C" {

extern PLUGIN_API SDL_WindowFlags PluginRenderApiGetWindowFlags();
extern PLUGIN_API render::Api* PluginRenderApiCreate(SDL_Window* window, float scale_factor);
extern PLUGIN_API void PluginRenderApiDestroy(render::Api* api);

} // extern "C"

#endif // RENDER_API_PLUGIN_H_
