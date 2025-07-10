#ifndef RENDER_ERROR_H_
#define RENDER_ERROR_H_

#include "base/error.h"

namespace render {

struct Error : base::Error {
  using base::Error::Error;
};

} // namespace render

#endif // RENDER_ERROR_H_
