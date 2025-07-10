#ifndef RESOURCE_SCOPE_EXIT_H_
#define RESOURCE_SCOPE_EXIT_H_

namespace resource {

template<typename ...Args>
static inline void release_all(Args&&... args) {
  ((args.release()), ...);
}

template<typename F>
class scope_exit {
public:
  explicit scope_exit(const F& func) : func_(func), released_(false) {}

  scope_exit() = delete;
  scope_exit(const scope_exit&) = delete;
  scope_exit(scope_exit&&) = delete;

  scope_exit& operator=(scope_exit&&) = delete;
  scope_exit& operator=(const scope_exit&) = delete;

  ~scope_exit() {
    if (!released_) {
      func_();
    }
  }

  void release() {
    released_ = true;
  }
private:
  F func_;
  bool released_;
};

template<typename F>
scope_exit(const F& f) -> scope_exit<F>;

} // namespace resource

#endif // RESOURCE_SCOPE_EXIT_H_