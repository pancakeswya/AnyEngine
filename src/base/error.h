#ifndef BASE_ERROR_H_
#define BASE_ERROR_H_

#include <stdexcept>

namespace base {

struct Error : std::runtime_error {
  using runtime_error::runtime_error;
};

} // namespace base

#endif // BASE_ERROR_H_
