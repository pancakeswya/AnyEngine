#ifndef RENDER_APIS_GL_LOADER_H_
#define RENDER_APIS_GL_LOADER_H_

#ifdef RENDER_OPENGL_ES2
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

#include <SDL3/SDL_video.h>

namespace gl {

static inline int InitLoader() {
#ifdef RENDER_OPENGL_ES2
  return gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);
#else
  return gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
#endif
}

} // namespace gl

#endif // RENDER_APIS_GL_LOADER_H_
