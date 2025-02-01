#ifndef EKG_UI_SCROLLBAR_HPP
#define EKG_UI_SCROLLBAR_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct scrollbar_theme_t {
  public:
    ekg::vec4<float> background {};
    ekg::vec4<float> outline {};
    ekg::vec4<float> activity {};
    ekg::vec4<float> highlight {};
    int32_t pixel_thickness {};
    float min_bar_size {};
  };

  struct scrollbar_t {
  public:
  };
}

#endif