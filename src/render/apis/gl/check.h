#ifndef RENDER_APIS_GL_CHECK_H_
#define RENDER_APIS_GL_CHECK_H_

#include "render/apis/gl/loader.h"

#ifndef NDEBUG
#include <SDL3/SDL_log.h>
#define GL_CHECK(_CALL) do { _CALL; GLenum gl_err = glGetError(); if (gl_err != 0) SDL_Log("GL error 0x%x, File: %s, Line: %d.\n", gl_err, __FILE__, __LINE__); } while (0)
#else
#define GL_CHECK(_CALL) _CALL
#endif

#endif // RENDER_APIS_GL_CHECK_H_
