#ifndef RENDER_APIS_GL_HANDLE_H_
#define RENDER_APIS_GL_HANDLE_H_

#include "base/decl.h"
#include "render/apis/gl/loader.h"

#include <utility>

namespace gl {

class ArrayHandle {
public:
  using CreateFuncType = void(*)(GLsizei, GLuint*);
  using DestroyFuncType = void(*)(GLsizei, const GLuint*);

  ArrayHandle() = default;

  explicit ArrayHandle(const CreateFuncType create,
                       const DestroyFuncType destroy,
                       const GLsizei size = 1)
    : size_(size), destroy_(destroy) {
    create(size, &handle_);
  }

  ArrayHandle(const ArrayHandle& other) = delete;

  ArrayHandle(ArrayHandle&& other) noexcept
    : handle_(std::exchange(other.handle_, GL_INVALID_VALUE)),
      size_(std::exchange(other.size_, 0)),
      destroy_(std::exchange(other.destroy_, nullptr)) {}

  ~ArrayHandle() {
    Destroy();
  }

  ArrayHandle& operator=(const ArrayHandle& other) = delete;

  ArrayHandle& operator=(ArrayHandle&& other) noexcept {
    if (*this != other) {
      Destroy();
      handle_ = std::exchange(other.handle_, GL_INVALID_VALUE);
      size_ = std::exchange(other.size_, 0);
      destroy_ = std::exchange(other.destroy_, nullptr);
    }
    return *this;
  }

  [[nodiscard]] operator GLuint() const noexcept {
    return handle_;
  }

  [[nodiscard]] GLsizei size() const noexcept {
    return size_;
  }
protected:
  GLuint handle_ = GL_INVALID_VALUE;
  GLsizei size_ = 0;
private:
  DestroyFuncType destroy_ = nullptr;

  void Destroy() noexcept {
    if (handle_ != GL_INVALID_VALUE) {
      destroy_(size_, &handle_);
      handle_ = GL_INVALID_VALUE;
    }
  }
};

class Handle {
public:
  using DestroyFuncType = void(*)(GLuint);

  Handle() noexcept : handle_(GL_INVALID_VALUE) {}

  explicit Handle(const GLuint handle, const DestroyFuncType destroy) noexcept
     : handle_(handle), destroy_(destroy) {}

  Handle(const Handle& other) = delete;

  Handle(Handle&& other) noexcept :
    handle_(std::exchange(other.handle_, GL_INVALID_VALUE)),
    destroy_(std::exchange(other.destroy_, nullptr)) {}

  ~Handle() {
    Destroy();
  }

  Handle& operator=(const Handle& other) = delete;

  Handle& operator=(Handle&& other) noexcept {
    if (this != &other) {
      Destroy();
      handle_ = std::exchange(other.handle_, GL_INVALID_VALUE);
      destroy_ = std::exchange(other.destroy_, nullptr);
    }
    return *this;
  }

  [[nodiscard]]operator GLuint() const noexcept {
    return handle_;
  }
protected:
  GLuint handle_;
private:
  DestroyFuncType destroy_;

  void Destroy() noexcept {
    if (handle_ != 0) {
      destroy_(handle_);
      handle_ = 0;
    }
  }
};

} // namespace gl

#endif // RENDER_APIS_GL_HANDLE_H_
