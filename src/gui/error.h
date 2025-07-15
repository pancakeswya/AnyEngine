#ifndef GUI_ERROR_H
#define GUI_ERROR_H

#include "base/error.h"

namespace gui {

struct Error final : base::Error {
  using base::Error::Error;
};

} // namespace gui

#endif // GUI_ERROR_H
