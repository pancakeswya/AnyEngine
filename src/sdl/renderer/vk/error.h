#ifndef SDL_RENDERER_VK_ERROR_H_
#define SDL_RENDERER_VK_ERROR_H_

#include "sdl/error.h"

namespace sdl::vk {

struct Error final : sdl::Error {
  using sdl::Error::Error;
};

} // namespace sdl::vk

#endif // SDL_RENDERER_VK_ERROR_H_
