#ifndef BASE_DECL_H_
#define BASE_DECL_H_

#define DECLARE_DEFAULT_NO_COPY_CLASS(HANDLE_NAME)               \
  HANDLE_NAME() noexcept = default;                              \
  HANDLE_NAME(const HANDLE_NAME& other) = delete;                \
  HANDLE_NAME(HANDLE_NAME&& other) noexcept = default;           \
  ~HANDLE_NAME() = default;                                      \
  HANDLE_NAME& operator=(const HANDLE_NAME& other) = delete;     \
  HANDLE_NAME& operator=(HANDLE_NAME&& other) noexcept = default

#define DECLARE_DEFAULT_CLASS(HANDLE_NAME)                       \
  HANDLE_NAME() noexcept = default;                              \
  HANDLE_NAME(const HANDLE_NAME& other) = default;               \
  HANDLE_NAME(HANDLE_NAME&& other) noexcept = default;           \
  ~HANDLE_NAME() = default;                                      \
  HANDLE_NAME& operator=(const HANDLE_NAME& other) = default;    \
  HANDLE_NAME& operator=(HANDLE_NAME&& other) noexcept = default

#endif // BASE_DECL_H_
