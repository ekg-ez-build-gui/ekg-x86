#ifndef EKG_UI_SLIDER_HPP
#define EKG_UI_SLIDER_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct slider_theme_t {
  public:
    ekg::vec4<float> background {};
    ekg::vec4<float> bar_background {};
    ekg::vec4<float> string {};
    ekg::vec4<float> outline {};
    ekg::vec4<float> activity {};
    ekg::vec4<float> activity_bar {};
    ekg::vec4<float> bar_outline {};
    ekg::vec4<float> highlight {};
    int32_t bar_thickness {};
    int32_t target_thickness {};
  };

  struct slider_t {
  public:
  };
}

#endif