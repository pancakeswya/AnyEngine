#ifndef RENDER_APIS_GL_BUFFER_H_
#define RENDER_APIS_GL_BUFFER_H_

#include "render/apis/gl/loader.h"
#include "render/apis/gl/handle.h"
#include "render/apis/gl/check.h"
#ifdef USE_BUFFER_MAP
#if !defined(glMapBuffer) && !defined(glMapBufferRange)
#error "No buffer map available"
#endif
#include "render/apis/gl/error.h"
#endif

namespace gl {

struct VertexArrayBuffer : ArrayHandle {
  VertexArrayBuffer(const VertexArrayBuffer& other) = delete;
  VertexArrayBuffer(VertexArrayBuffer&& other) noexcept = default;
  VertexArrayBuffer& operator=(const VertexArrayBuffer& other) = delete;
  VertexArrayBuffer& operator=(VertexArrayBuffer&& other) noexcept = default;
  ~VertexArrayBuffer() = default;

  VertexArrayBuffer() 
    : ArrayHandle(glGenVertexArrays, glDeleteVertexArrays) {}

  void Bind() const {
    GL_CHECK(glBindVertexArray(handle_));
  }

  void Unbind() const {
    GL_CHECK(glBindVertexArray(0));
  }
};

class Buffer : public ArrayHandle {
public:
  DECLARE_DEFAULT_NO_COPY_CLASS(Buffer);
#ifdef USE_BUFFER_MAP
  enum MapBit : GLenum {
#if defined(glMapBufferRange)
    kWriteBit = GL_MAP_WRITE_BIT,
    kReadBit = GL_MAP_READ_BIT
#else
    kWriteBit = GL_WRITE_ONLY,
    kReadBit = GL_READ_ONLY
#endif
  };
#endif

  explicit Buffer(const GLenum type, const GLsizei size = 1)
    : ArrayHandle(glGenBuffers, glDeleteBuffers, size), type_(type) {}

  void Bind() const {
    GL_CHECK(glBindBuffer(type_, handle_));
  }

  void Unbind() const {
    GL_CHECK(glBindBuffer(type_, 0));
  }

  void Allocate(const size_t size, const GLenum usage) {
    GL_CHECK(glBufferData(type_, static_cast<GLsizeiptr>(size),  nullptr, usage));
    allocated_size_ = size;
  }

  [[nodiscard]] size_t allocated_size() const noexcept { return allocated_size_; }
#ifdef USE_BUFFER_MAP
  template<typename T>
  T* Map(const MapBit access) const {
#if defined(glMapBufferRange)
    void* mapped = glMapBufferRange(type_, 0,  static_cast<GLsizeiptr>(allocated_size_), access);
#else
    void* mapped = glMapBuffer(type_, access);
#endif
    if (mapped == nullptr) {
      throw Error("Failed to map indices");
    }
    return static_cast<T*>(mapped);
  }

  void Unmap() const {
    glUnmapBuffer(type_);
  }
#else
  template<typename T>
  void Copy(const T* data, const size_t size) const {
    GL_CHECK(glBufferSubData(type_, 0,  static_cast<GLsizeiptr>(size), static_cast<const void*>(data)));
  }
#endif

private:
  GLenum type_ = GL_INVALID_VALUE;
  size_t allocated_size_ = 0;
};

} // namespace gl

#endif // RENDER_APIS_GL_BUFFER_H_
