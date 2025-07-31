#ifndef RENDER_APIS_GL_WINDOW_H_
#define RENDER_APIS_GL_WINDOW_H_

namespace gl {

struct Window {
  virtual ~Window() = default;
  virtual void GetSize(int& width, int& height) const noexcept = 0;
};

} // namespace gl

#endif // RENDER_APIS_GL_WINDOW_H_
