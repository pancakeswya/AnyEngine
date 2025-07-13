#ifndef RENDER_APIS_GL_CONTEXT_H_
#define RENDER_APIS_GL_CONTEXT_H_

#include "render/apis/gl/handle.h"

#include <SDL3/SDL.h>

namespace gl {

struct Context : RUNTIME_HANDLE(SDL_GLContext, &SDL_GL_DestroyContext) {
  explicit Context(SDL_Window* window);
  ~Context() = default;
};

} // namespace

#endif // RENDER_APIS_GL_CONTEXT_H_
