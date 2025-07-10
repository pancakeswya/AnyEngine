#ifndef IO_ERROR_H_
#define IO_ERROR_H_

#include "base/error.h"

namespace io {

struct Error final : base::Error {
  using base::Error::Error;
};

} // namespace io

#endif // IO_ERROR_H_