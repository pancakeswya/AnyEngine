#ifndef RENDER_APIS_GL_BUFFER_H_
#define RENDER_APIS_GL_BUFFER_H_

#include "render/apis/gl/loader.h"
#include "render/apis/gl/handle.h"
#include "render/apis/gl/error.h"

namespace gl {

class Buffer : public ArrayHandle<glGenBuffers, glDeleteBuffers> {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Buffer);

  explicit Buffer(const GLenum type, const GLsizei size = 1)
    : ArrayHandle(size), type_(type) {}

  void Bind() const {
    glBindBuffer(type_, handle_);
  }

  void Allocate(const size_t size, const GLenum usage) const {
    glBufferData(type_, static_cast<GLsizeiptr>(size),  nullptr, usage);
  }

  template<typename T>
  T* Map(const GLenum access) const {
    void* mapped = glMapBuffer(type_, access);
    if (mapped == nullptr) {
      throw Error("Failed to map indices");
    }
    return static_cast<T*>(mapped);
  }

  void Unmap() const {
    glUnmapBuffer(type_);
  }

private:
  GLenum type_ = GL_INVALID_VALUE;
};

} // namespace gl

#endif // RENDER_APIS_GL_BUFFER_H_
