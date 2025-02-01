#ifndef EKG_UI_CHECKBOX_HPP
#define EKG_UI_CHECKBOX_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct checkbox_theme_t {
  public:
    ekg::vec4<float> background {};
    ekg::vec4<float> string {};
    ekg::vec4<float> outline {};
    ekg::vec4<float> activity {};
    ekg::vec4<float> highlight {};
  };

  struct checkbox_t {
  public:
  };
}

#endif