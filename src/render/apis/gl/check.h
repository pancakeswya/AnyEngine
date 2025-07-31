#ifndef RENDER_APIS_GL_CHECK_H_
#define RENDER_APIS_GL_CHECK_H_

#ifndef ANY_RELEASE
#include "render/apis/gl/loader.h"
using LogCallbackType = void (*)(const char* fmt, ...);
extern LogCallbackType gl_log_callback;
#define GL_CHECK(_CALL) do { _CALL; GLenum gl_err = glGetError(); if (gl_err != 0) gl_log_callback("GL error 0x%x, File: %s, Line: %d.\n", gl_err, __FILE__, __LINE__); } while (0)
#else
#define GL_CHECK(_CALL) _CALL
#endif

#endif // RENDER_APIS_GL_CHECK_H_
