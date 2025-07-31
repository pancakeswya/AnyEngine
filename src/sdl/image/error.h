#ifndef SDL_IMAGE_ERROR_H_
#define SDL_IMAGE_ERROR_H_

#include "base/error.h"

namespace sdl {

struct Error : base::Error {
  using base::Error::Error;
};

} // namespace sdl

#endif // SDL_IMAGE_ERROR_H_
