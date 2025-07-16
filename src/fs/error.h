#ifndef FS_ERROR_H_
#define FS_ERROR_H_

#include "base/error.h"

namespace fs {

struct Error final : base::Error {
  using base::Error::Error;
};

} // namespace fs

#endif // FS_ERROR_H_