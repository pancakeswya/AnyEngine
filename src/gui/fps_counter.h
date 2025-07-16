#ifndef GUI_FPS_COUNTER_H_
#define GUI_FPS_COUNTER_H_

namespace gui {

class FpsCounter {
public:
  double Count() noexcept;
private:
  double fps_ = 0.0;

  double last_update_time_ = 0.0;
  int frames_since_last_update_ = 0;
};

} // namespace gui

#endif // GUI_FPS_COUNTER_H_
