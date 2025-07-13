#include "render/apis/gl/context.h"

#include "render/apis/gl/error.h"

#include <string>

namespace gl {

namespace {

SDL_GLContext CreateContext(SDL_Window* window) {
#ifdef RENDER_IMPL_OPENGL_ES3
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
# if defined(__APPLE__)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
# else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
# endif
#endif
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

  SDL_GLContext context = SDL_GL_CreateContext(window);
  if (context == nullptr) {
    throw Error(std::string("Failed to create SDL context with error:") + SDL_GetError());
  }
  if (!InitLoader()) {
    throw Error("Failed to load GL/GLES");
  }
  return context;
}

} // namespace

Context::Context(SDL_Window* window) 
  : Handle(CreateContext(window), SDL_GL_DestroyContext) {
  SDL_GL_MakeCurrent(window, handle_);
  SDL_GL_SetSwapInterval(1);
}

} // namespace gl