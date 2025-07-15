#ifndef RENDER_FPS_COUNTER_H_
#define RENDER_FPS_COUNTER_H_

#include <chrono>

namespace render {

class FpsCounter {
public:
  double Count() noexcept {
    const double now = std::chrono::duration_cast<std::chrono::duration<double>>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
      ).count();
    frames_since_last_update_++;

    if (now - last_update_time_ > 0.25) {
      fps_ = frames_since_last_update_ / (now - last_update_time_);
      last_update_time_ = now;
      frames_since_last_update_ = 0;
    }
    return fps_;
  }
private:
  double fps_ = 0.0;

  double last_update_time_ = 0.0;
  int frames_since_last_update_ = 0;
};

} // namespace render

#endif // RENDER_FPS_COUNTER_H_
