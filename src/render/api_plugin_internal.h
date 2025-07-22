#ifndef RENDER_API_PLUGIN_INTERNAL_H_
#define RENDER_API_PLUGIN_INTERNAL_H_

#include "render/api.h"
#include "resource/scope_exit.h"

#include <SDL3/SDL.h>

#include <string>

namespace render::internal {

template<typename ApiType, typename ApiErrorType>
struct ApiPlugin {
  static Api* CreateApiAndWindow(
    const char* title,
    const int width,
    const int height,
    const SDL_WindowFlags window_flags,
    SDL_Window*& window
  ) {
    if (!SDL_Init(SDL_INIT_VIDEO)){
      throw ApiErrorType(std::string("SDL_Init failed, error: ") + SDL_GetError());
    }
    resource::scope_exit sdl_guard([] {
      SDL_Quit();
    });
    const float scale_factor = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    window = SDL_CreateWindow(
      title,
      static_cast<int>(static_cast<float>(width) * scale_factor),
      static_cast<int>(static_cast<float>(height) * scale_factor),
      window_flags
    );
    if (window == nullptr) {
      throw ApiErrorType(std::string("Failed to create window, error: ") + SDL_GetError());
    }
    resource::scope_exit window_guard([window] {
      SDL_DestroyWindow(window);
    });
    const auto api = new ApiType(window, scale_factor);
    resource::release_all(
      sdl_guard,
      window_guard
    );
    return api;
  }

  static void Destroy(Api* api, SDL_Window* window) {
    delete api;
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
};

} // namespace render::internal

#endif // RENDER_API_PLUGIN_INTERNAL_H_
