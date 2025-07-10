#ifndef RENDER_TRANSFERERS_SDL_ERROR_H_
#define RENDER_TRANSFERERS_SDL_ERROR_H_

#include "render/error.h"

namespace sdl {

struct Error final : render::Error {
  using render::Error::Error;
};

} // namespace sdl

#endif // RENDER_TRANSFERERS_SDL_ERROR_H_
