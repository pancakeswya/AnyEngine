#ifndef SDL_ERROR_H_
#define SDL_ERROR_H_

#include "base/error.h"

#include <SDL3/SDL.h>

#include <string>

namespace sdl {

struct Error : base::Error {
  using base::Error::Error;

  [[nodiscard]] Error WithMessage() const {
    return Error{std::string(what()) + " [Message: " + std::string(SDL_GetError()) + ']'};
  }
};

} // namespace render

#endif // SDL_ERROR_H_
