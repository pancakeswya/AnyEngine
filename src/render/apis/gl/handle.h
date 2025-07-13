#ifndef RENDER_APIS_GL_HANDLE_H_
#define RENDER_APIS_GL_HANDLE_H_

#include "base/decl.h"
#include "render/apis/gl/loader.h"

#include <utility>

#define RUNTIME_HANDLE(TYPE1, DESTROY_FUNC) Handle<TYPE1, decltype(DESTROY_FUNC)>

namespace gl {

class ArrayHandle {
public:
  using CreateFuncType = void(*)(GLsizei, GLuint*);
  using DestroyFuncType = void(*)(GLsizei, const GLuint*);

  explicit ArrayHandle(CreateFuncType create, DestroyFuncType destroy, const GLsizei size = 1) 
    : size_(size), destroy_(destroy) {
    create(size, &handle_);
  }

  ArrayHandle(const ArrayHandle& other) = delete;

  ArrayHandle(ArrayHandle&& other) noexcept
    : handle_(std::exchange(other.handle_, GL_INVALID_VALUE)),
      size_(std::exchange(other.size_, 0)) {}

  ~ArrayHandle() {
    Destroy();
  }

  ArrayHandle& operator=(const ArrayHandle& other) = delete;

  ArrayHandle& operator=(ArrayHandle&& other) noexcept {
    if (*this != other) {
      Destroy();
      handle_ = std::exchange(other.handle_, GL_INVALID_VALUE);
      size_ = std::exchange(other.size_, 0);
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
  DestroyFuncType destroy_;

  void Destroy() noexcept {
    if (handle_ != GL_INVALID_VALUE) {
      destroy_(size_, &handle_);
      handle_ = GL_INVALID_VALUE;
    }
  }
};

template<typename HandleType, typename DestroyFuncType>
class Handle {
public:
  Handle() noexcept : handle_(0) {}

  explicit Handle(HandleType handle, DestroyFuncType destroy) noexcept
     : handle_(handle), destroy_(destroy) {}

  Handle(const Handle& other) = delete;

  Handle(Handle&& other) noexcept :
    handle_(std::exchange(other.handle_, 0)) {}

  ~Handle() {
    Destroy();
  }

  Handle& operator=(const Handle& other) = delete;

  Handle& operator=(Handle&& other) noexcept {
    if (this != &other) {
      Destroy();
      handle_ = std::exchange(other.handle_, 0);
    }
    return *this;
  }

  [[nodiscard]]operator HandleType() const noexcept {
    return handle_;
  }
protected:
  HandleType handle_;
private:
  DestroyFuncType destroy_;

  void Destroy() noexcept {
    if (handle_ != 0) {
      destroy_(handle_);
    }
  }
};

} // namespace gl

#endif // RENDER_APIS_GL_HANDLE_H_
