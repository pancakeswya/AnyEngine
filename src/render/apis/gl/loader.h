#ifndef RENDER_APIS_GL_LOADER_H_
#define RENDER_APIS_GL_LOADER_H_

#ifdef USE_OPENGL_ES3
#include <glad/gles2.h>
#else
#include <glad/gl.h>
#endif

namespace gl {

static inline int InitLoader(GLADloadfunc load_func) {
#ifdef USE_OPENGL_ES3
  return gladLoadGLES2(load_func);
#else
  return gladLoadGL(load_func);
#endif
}

} // namespace gl

#endif // RENDER_APIS_GL_LOADER_H_
