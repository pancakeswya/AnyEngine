#ifndef RENDER_PARSERS_OBJ_ERROR_H_
#define RENDER_PARSERS_OBJ_ERROR_H_

#include "base/error.h"

namespace obj {

struct Error final : base::Error {
  using base::Error::Error;
};

} // namespace obj

#endif // RENDER_PARSERS_OBJ_ERROR_H_