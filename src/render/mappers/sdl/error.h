#ifndef RENDER_MAPPERS__SDL_ERROR_H_
#define RENDER_MAPPERS__SDL_ERROR_H_

#include "render/error.h"

namespace sdl {

struct Error final : render::Error {
  using render::Error::Error;
};

} // namespace sdl

#endif // RENDER_MAPPERS__SDL_ERROR_H_
