#ifndef EKG_UI_BUTTON_HPP
#define EKG_UI_BUTTON_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct button_theme_t {
  public:
    ekg::vec4<float> background {};
    ekg::vec4<float> string {};
    ekg::vec4<float> outline {};
    ekg::vec4<float> activity {};
    ekg::vec4<float> highlight {};
  };

  struct button_t {
  public:
  };
}

#endif