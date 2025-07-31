#ifndef GUI_RENDERER_H_
#define GUI_RENDERER_H_

#include "gui/fps_counter.h"

namespace gui {

class Renderer {
public:
  explicit Renderer(float scale_factor);
  virtual ~Renderer();
  virtual void RenderFrame();
private:
  FpsCounter fps_counter_;
};

} // namespace gui

#endif // GUI_RENDERER_H_
