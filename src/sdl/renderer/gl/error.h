#ifndef SDL_RENDERER_GL_ERROR_H_
#define SDL_RENDERER_GL_ERROR_H_

#include "sdl/error.h"

namespace sdl::gl {

struct Error final : sdl::Error {
  using sdl::Error::Error;
};

} // namespace sdl::gl

#endif // SDL_RENDERER_GL_ERROR_H_
