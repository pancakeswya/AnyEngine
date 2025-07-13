#ifndef RENDER_APIS_GL_LOADER_H_
#define RENDER_APIS_GL_LOADER_H_

#define GL_GLEXT_PROTOTYPES
#if defined(RENDER_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#endif // RENDER_APIS_GL_LOADER_H_
