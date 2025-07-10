#ifndef RENDER_APIS_VK_HANDLE_H_
#define RENDER_APIS_VK_HANDLE_H_

#include "base/decl.h"

#include <vulkan/vulkan.h>

#include <algorithm>
#include <functional>

namespace vk {

template<typename HandleType>
using NonDispatchableArrayHandle = std::unique_ptr<HandleType[], std::function<void(HandleType[])>>;

template<typename HandleType>
void NoDestroy([[maybe_unused]]HandleType handle, [[maybe_unused]]const VkAllocationCallbacks* callbacks) {}

template<typename HandleType, void(*DestroyFunc)(HandleType handle, const VkAllocationCallbacks* callbacks)>
class Handle {
public:
  Handle() noexcept : handle_(VK_NULL_HANDLE) {}

  explicit Handle(HandleType handle) noexcept
     : handle_(handle) {}

  Handle(const Handle& other) = delete;

  Handle(Handle&& other) noexcept :
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE)) {}

  ~Handle() {
    Destroy();
  }

  Handle& operator=(const Handle& other) = delete;

  Handle& operator=(Handle&& other) noexcept {
    if (this != &other) {
      Destroy();
      handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);
    }
    return *this;
  }

  [[nodiscard]]operator HandleType() const noexcept {
    return handle_;
  }
protected:
  HandleType handle_;
private:
  void Destroy() noexcept {
    if (handle_ != VK_NULL_HANDLE) {
      DestroyFunc(handle_, nullptr);
    }
  }
};

template<typename HandleType, typename ParentType, void(*DestroyFunc)(ParentType parent, HandleType handle, const VkAllocationCallbacks* callbacks)>
class NonDispatchableHandle {
public:
  NonDispatchableHandle() noexcept
    : handle_(VK_NULL_HANDLE),
      parent_(VK_NULL_HANDLE) {}

  NonDispatchableHandle(HandleType handle, ParentType parent) noexcept
    : handle_(handle), parent_(parent) {}

  NonDispatchableHandle(const NonDispatchableHandle& other) = delete;

  NonDispatchableHandle(NonDispatchableHandle&& other) noexcept :
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE)),
    parent_(std::exchange(other.parent_, VK_NULL_HANDLE)) {}

  ~NonDispatchableHandle() {
    Destroy();
  }

  NonDispatchableHandle& operator=(const NonDispatchableHandle& other) = delete;

  NonDispatchableHandle& operator=(NonDispatchableHandle&& other) noexcept {
    if (this != &other) {
      Destroy();
      handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);
      parent_ = std::exchange(other.parent_, VK_NULL_HANDLE);
    }
    return *this;
  }

  [[nodiscard]]operator HandleType() const noexcept {
    return handle_;
  }
protected:
  HandleType handle_;
  ParentType parent_;
private:
  void Destroy() noexcept {
    if (handle_ != VK_NULL_HANDLE && parent_ != VK_NULL_HANDLE) {
      DestroyFunc(parent_, handle_, nullptr);
    }
  }
};

template<typename HandleType, typename ParentType>
class NonDispatchableRuntimeHandle {
public:
  using DestroyFunc = void(*)(ParentType parent, HandleType handle, const VkAllocationCallbacks* callbacks);
  NonDispatchableRuntimeHandle() noexcept
    : handle_(VK_NULL_HANDLE),
      parent_(VK_NULL_HANDLE),
      destroy_func_(nullptr) {}

  NonDispatchableRuntimeHandle(HandleType handle, ParentType parent, const DestroyFunc destroy_func) noexcept
    : handle_(handle), parent_(parent), destroy_func_(destroy_func) {}

  NonDispatchableRuntimeHandle(const NonDispatchableRuntimeHandle& other) = delete;

  NonDispatchableRuntimeHandle(NonDispatchableRuntimeHandle&& other) noexcept :
    handle_(std::exchange(other.handle_, VK_NULL_HANDLE)),
    parent_(std::exchange(other.parent_, VK_NULL_HANDLE)),
    destroy_func_(std::exchange(other.destroy_func_, nullptr)){}

  ~NonDispatchableRuntimeHandle() {
    Destroy();
  }

  NonDispatchableRuntimeHandle& operator=(const NonDispatchableRuntimeHandle& other) = delete;

  NonDispatchableRuntimeHandle& operator=(NonDispatchableRuntimeHandle&& other) noexcept {
    if (this != &other) {
      Destroy();
      handle_ = std::exchange(other.handle_, VK_NULL_HANDLE);
      parent_ = std::exchange(other.parent_, VK_NULL_HANDLE);
    }
    return *this;
  }

  [[nodiscard]]operator HandleType() const noexcept {
    return handle_;
  }
protected:
  HandleType handle_;
  ParentType parent_;
  DestroyFunc destroy_func_;
private:
  void Destroy() noexcept {
    if (handle_ != VK_NULL_HANDLE && parent_ != VK_NULL_HANDLE) {
      destroy_func_(parent_, handle_, nullptr);
    }
  }
};

template<typename HandleType>
class IdentityHandle {
public:
  DECLARE_DEFAULT_CLASS(IdentityHandle);

  explicit IdentityHandle(HandleType handle) noexcept : handle_(handle) {}

  [[nodiscard]]operator HandleType() const noexcept {
    return handle_;
  }
protected:
  HandleType handle_ = VK_NULL_HANDLE;
};

} // namespace vk

#endif // RENDER_APIS_VK_HANDLE_H_
