#ifndef GUI_RENDERER_H_
#define GUI_RENDERER_H_

namespace gui {

struct Renderer {
  explicit Renderer(float scale_factor);
  virtual ~Renderer();
  virtual void RenderFrame() const;
};

} // namespace gui

#endif // GUI_RENDERER_H_
