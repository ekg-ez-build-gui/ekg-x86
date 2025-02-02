#ifndef EKG_UI_FRAME_HPP
#define EKG_UI_FRAME_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct frame_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> border {};
    ekg::vec4_t<float> outline {};
    int32_t activity_offset {};
  };

  struct frame_t {
  public:
  };
}

#endif